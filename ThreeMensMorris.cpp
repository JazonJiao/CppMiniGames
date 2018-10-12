//
// Created by 焦玉华 on 2018/6/2.
//

#include <iostream>
#include "ThreeMensMorris.h"

enum state {EMPTY, P1, P2};

//#define show(a, b) (states[a][b] == EMPTY ? " " : (states[a][b] == P1 ? "X" : "O"))

using namespace std;

ThreeMensMorris::ThreeMensMorris() : curPlayer(false) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++)
            states[i][j] = EMPTY;
    }
    cout << "Welcome to Three Men's Morris!\n"
            "This game is similar to Tic-tac-toe,\n"
            "except after each player placed 3 pieces,\n"
            "They can't add new pieces, but can\n"
            "move pieces along the lines. Good luck! " << endl;
    play();
}

char ThreeMensMorris::ch(int row, int col) {
    state s = (state) states[row][col];
    if (s == EMPTY)
        return ' ';
    else if (s == P1)
        return 'X';
    else
        return 'O';
}

void ThreeMensMorris::display() {
    cout << ch(0, 2) << "-" << ch(1, 2) << "-" << ch(2, 2) << endl;
    cout << " \\|/" << endl;
    cout << ch(0, 1) << "-" << ch(1, 1) << "-" << ch(2, 1) << endl;
    cout << " /|\\" << endl;
    cout << ch(0, 0) << "-" << ch(1, 0) << "-" << ch(2, 0) << endl;
}


bool ThreeMensMorris::valid1(int row, int col) {
    return row > -1 && row < WIDTH && col > -1 && col < WIDTH && states[row][col] == EMPTY;
}

// user plays a round before 6 pieces are placed
void ThreeMensMorris::play1() {
    int curRow, curCol;

    display();
    cout << "Player " << curPlayer + 1 << ", enter your next move: ";

    /// Error log: could have had the user enter a number from 1 to 9 (or 0 to 8)
    /// and then translate into 2-D location. This might be more convenient.
    /// Demonstrates the importance of prior program design! ---180602

    cin >> curRow >> curCol;
    while (not valid1(curRow, curCol)) {
        cout << "Invalid input. Try again: ";
        cin >> curRow >> curCol;
    }
    states[curRow][curCol] = (curPlayer ? P2 : P1);

    curPlayer = not curPlayer;
}

bool ThreeMensMorris::valid2(int fr, int fc, int tr, int tc) {
    if (fr < 0 || fr >= WIDTH || fc < 0 || fc >= WIDTH || states[fr][fc] != (curPlayer ? P2 : P1))
        return false;
    if (tr < 0 || tr >= WIDTH || tc < 0 || tc >= WIDTH || states[tr][tc] != EMPTY)
        return false;
    //return (tr >= fr-1 && tr <= fr+1 && tc >= fc-1 && tc <= fc+1);

    /// creative new algorithm!
    int lrd = abs(tc - fc);  // difference between x-coord
    int udd = abs(tr - fr);  // difference between y-coord
    if (abs(fc - fr) == 1) {  // original piece not on diagonal or center
        return lrd + udd == 1;  // either 0 1 or 1 0
    } else {
        return (lrd < 2) && (udd < 2);  // can be 0 1, 1 0, or 1 1
        /// This becomes convenient for single-array setup: if location % 2 != 0...
    }
}

// user plays a round after 6 pieces are placed
bool ThreeMensMorris::play2() {
    int frrow, frcol, torow, tocol;

    display();
    cout << "Player " << curPlayer+1 << ", enter your next move: ";
    cin >> frrow >> frcol >> torow >> tocol;
    if (frrow == -1)
        return true;
    while (not valid2(frrow, frcol, torow, tocol)) {
        cout << "Invalid input. Try again: ";
        cin >> frrow >> frcol >> torow >> tocol;
    }
    states[frrow][frcol] = EMPTY;
    states[torow][tocol] = curPlayer ? P2 : P1;

    curPlayer = not curPlayer;
    return false;
}

// user plays
void ThreeMensMorris::play() {
    for (int i = 0; i < 5; i++) {
        play1();
    }
    if (not won()) {  // at the 5th round, player 1 might've won
        play1();
        displayInfo();
    } else {
        display();
        cout << "Player " << (curPlayer ? P1 : P2) << ", congrats! You won! " << endl;
        return;
    }

    bool endGame = false;
    while (not endGame and not won()) {
        endGame = play2();
    }
    if (endGame) {
        cout << "Game aborted." << endl;
    } else {
        display();
        cout << "Player " << (curPlayer ? P1 : P2) << ", congrats! You won! " << endl;
    }
}

int ThreeMensMorris::step1() {
    display();
    states[1][1] = P1;
    cout << "Computer chooses (1, 1)." << endl;

    int row, col;
    display();
    cout << "Please enter your next move: ";
    cin >> row >> col;
    while (not valid1(row, col)) {
        cout << "Invalid input. Try again: ";
        cin >> row >> col;
    }
    return row + col;
}

void ThreeMensMorris::play3(int row, int col) {

};

// computer plays Player 1. Uses a must-win algorithm.
void ThreeMensMorris::autoplay() {  /// Error Log: in terms of structure, using inheritance might be better
    int rowPlusCol = step1();

    if ((rowPlusCol) % 2 == 1) {

    }
}

void ThreeMensMorris::displayInfo() {
    cout << "\nNext, for each move, enter 4 numbers:\n"
            "Two for the coordinates of what you want to move;\n"
            "Then, two for the coordinates of its destination.\n"
            "To quit the game, enter -1 0 0 0.\n\n";
}

bool ThreeMensMorris::won() {
    state player = curPlayer ? P1 : P2;
    if (states[1][1] == player) {
        /// Error log: do not use a == b == c!
        if (states[0][0] == player && states[2][2] == player)
            return true;
        if (states[1][0] == player && states[1][2] == player)
            return true;
        if (states[0][1] == player && states[2][1] == player)
            return true;
        if (states[2][0] == player && states[0][2] == player)
            return true;
    } else if (states[0][0] == player) {
        if (states[0][1] == player && states[0][2] == player)
            return true;
        if (states[1][0] == player && states[2][0] == player)
            return true;
    } else if (states[2][2] == player) {
        if (states[0][2] == player && states[1][2] == player)
            return true;
        if (states[2][0] == player && states[2][1] == player)
            return true;
    }
    return false;

    // a more troublesome algorithm: add all x-coord and y-coord of 3 pieces,
    // return x%3 == 0 && y%3 == 0; there are 2 exceptions
}


