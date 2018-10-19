//
// Written on 2018/5/24.
//

#include "Maxim.h"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cassert>
#include <fstream>

#define random(a, b) (rand() % (b-a+1) + a)

using std::cout;
using std::endl;
using std::cin;

// GAMEPLAY
// the game is played on a 7 * 7 grid, each grid having a number initialized randomly from 0 to 9.
// the central grid is initialized to 0, and Player 1 starts with it marking an "O" on it.
// then Player 2 chooses a grid that's either on the same row or the same column as
// the grid just chosen, adds the number on that grid to his/her score, and marking an "X" on it.
// Then the two players take turns to choose grids until all grids are chosen.
// The player with a higher total score wins.
// if one-player mode is chosen, computer is Player 1.

Maxim::Maxim() {
    int mode, sqL, difficulty;
    cout << "Welcome to Maxim! Enter Mode--\n"
            "0, Zero-player; \n"
            "1, One-Player; \n"
            "2, Two players: \n";
    cin >> mode;
    assert(mode == 0 or mode == 1 or mode == 2);
    cout << "\nChoose grid length "
            "\n(must be 3, 5, 7, or 9): \n";
    cin >> sqL;
    assert(sqL > 2 and sqL < 10 and sqL % 2 == 1);

    switch (mode) {
        case 0: {
            MaximZeroPlayer m0(sqL, 4);
            break;
        } case 1: {
            cout << "\nChoose difficulty (Must be "
                    "\nan even number less than grid size): \n";
            cin >> difficulty;
            assert(difficulty % 2 == 0 && difficulty < sqL);
            MaximOnePlayer m1(sqL, difficulty);
            break;
        } case 2: {
            MaximTwoPlayers m2(sqL);
        }
    }
}


MaximBase::MaximBase(int squareLength) :
        curRow(squareLength / 2), curCol(squareLength / 2), curPlayer(true), score1(0), score2(0) {
    srand((unsigned) time(nullptr));
    for (int i = 0; i < MAXIMLENGTH; i++) {
        for (int j = 0; j < MAXIMLENGTH; j++) {
            numbers[i][j] = random(0, 9);
        }
    }
    numbers[curRow][curCol] = 0;  // set central grid to be 0
    for (int i = 0; i < MAXIMLENGTH; i++) {
        for (int j = 0; j < MAXIMLENGTH; j++) {
            states[i][j] = EMPTY;
        }
    }
    states[curRow][curCol] = P1;
    //while (play());  // fixme: is this undefined?
}

void MaximBase::mark(int row, int col) {
    states[row][col] = curPlayer ? P2 : P1;
    if (!curPlayer) score1 += numbers[row][col];
    else score2 += numbers[row][col];
}

// when displaying, to use Cartesian coordinates,
// the row and column are swapped and flipped
void MaximBase::display() const {
    cout << "^ y-axis" << endl;
    for (int i = squareLength - 1; i >= 0; i--) {
        for (int j = 0; j < squareLength; j++) {
            if (states[j][i] == EMPTY) {
                cout << "|" << numbers[j][i];
            } else if (states[j][i] == P1) {
                cout << "|-";
            } else {
                cout << "| ";
            }
        }
        cout << "|";
        if (i > 0) {
            cout << endl;
        } else {
            cout << "-> x-axis\n";
        }
    }
    cout << "Current Grid: " << curRow << "," << curCol << "; Current Player: " << curPlayer + 1;
    cout << "\nScores--Player 1: " << score1 << "; Player 2: " << score2 << endl;
}

//void Maxim::mark2(int row, int col) {
//    states[row][col] = P2;
//    score2 += numbers[row][col];
//}

// ************************************ play() methods

bool MaximZeroPlayer::play() {
    computerChooses();
    display();
    curPlayer = not curPlayer;   // flip turns
    return not gameOver();
}

bool MaximOnePlayer::play() {
    if (curPlayer) {
        playerChooses();
    } else {
        computerChooses();
    }
    display();
    curPlayer = not curPlayer;   // flip turns
    return not gameOver();
}

bool MaximTwoPlayers::play() {
    playerChooses();
    display();
    curPlayer = not curPlayer;   // flip turns
    return not gameOver();
}

//
void MaximAuto::computerChooses() {
    int coord = chooseGrid(curRow, curCol, difficulty);
    curRow = coord / 100;
    curCol = coord % 100;
    mark(curRow, curCol);
}

void MaximBase::playerChooses() {
    int row, col;
    bool valid = false;
    while (!valid) {
        cout << "\nPlayer " << curPlayer + 1 << ", enter your next move: ";
        cin >> row >> col;
        if (isValid(row, col)) {
            valid = true;
        } else {
            cout << "Invalid input; try again!\n";
        }
    }
    curRow = row;
    curCol = col;  // assign inputs
    mark(curRow, curCol);
    cout << endl;
}


// tests whether the grid coordinates the user enters is valid
// the user's grid should be on the same row or column as the current grid
// if all grids on the row or column are occupied, the user can choose from any remaining grid
bool MaximBase::isValid(int row, int col) {
    if (row >= squareLength || row < 0 || col >= squareLength || col < 0)
        return false;
    bool hasEmpty = false;              // whether there are empty grids on the same row/col
    for (int i = 0; i < squareLength; i++) {
        if (states[curRow][i] == EMPTY) hasEmpty = true;
    }
    for (int i = 0; i < squareLength; i++) {
        if (states[i][curCol] == EMPTY) hasEmpty = true;
    }
    if (hasEmpty) {
        return (row == curRow || col == curCol) && states[row][col] == EMPTY;
    } else {
        return states[row][col] == EMPTY;
    }
}


// helper recursive method for chooseGridHard()
// returns the total net points after a certain iteration of optimized plays,
// not including the point of the first grid that's passed from outside
// iter is the number of iterations (n-ply) for how deep the algorithm goes; should be even
int MaximAuto::calcPoints(int row, int col, int iter) {
    if (iter == 0) {
        return 0;
    } else {
        states[row][col] = P1;
        int coord = chooseGridEasy(row, col);
        if (coord == -1) {
            states[row][col] = EMPTY;
            return 0;
        }
        int nextRow = coord / 100, nextCol = coord % 100;

        // if it's the other player's hypothesized turn (iter % 2 == 0),
        // the score should be subtracted (== 0 because player 1 plays the last round, when iter == 1)
        int score = numbers[nextRow][nextCol] * (iter % 2 == 0 ? -1 : 1);
        //cout << iter << "  " << nextRow << " " << nextCol << " " << score << endl;
        score += calcPoints(nextRow, nextCol, iter - 1);  // recursive call

        states[row][col] = EMPTY;
        return score;
    }
}

// iter is the number of iterations (n-ply) for how deep the algorithm goes; should be even
int MaximAuto::chooseGrid(int row, int col, int iter) {   // modified from chooseGridEasy()
    int NONE = -1000;
    int score = 0, curBiggest = NONE;
    int ansRow = row, ansCol = col;

    for (int i = 0; i < squareLength; i++) {
        if (states[row][i] == EMPTY) {
            score = numbers[row][i] + calcPoints(row, i, iter);
            //cout << row << " " << i << " " << score << endl;
            if (score > curBiggest) {
                curBiggest = score;
                ansCol = i;   // row already == curRow
            }
        }
    }
    for (int i = 0; i < squareLength; i++) {
        if (states[i][col] == EMPTY) {
            score = numbers[i][col] + calcPoints(i, col, iter);
            //cout << i << " " << col << " " << score << endl;
            if (score > curBiggest) {
                curBiggest = score;
                ansCol = col;   // resets column chosen
                ansRow = i;
            }
        }
    }
    if (curBiggest == NONE) {   // there are no empty grids on same row/col
        for (int i = 0; i < squareLength; i++) {
            for (int j = 0; j < squareLength; j++) {
                if (states[i][j] == EMPTY) {
                    score = numbers[i][j] + calcPoints(i, j, iter);
                    //cout << i << " " << j << " " << score << endl;
                    if (score > curBiggest) {
                        curBiggest = score;
                        ansRow = i;
                        ansCol = j;
                    }
                }
            }
        }
    }
    assert(curBiggest != NONE);
    return ansRow * 100 + ansCol;
}


// easy algorithm: one-ply (search for the largest number available)
// if game is over, return -1 (should not happen);
// else returns a three-digit number; the first digit is row, last is col
int MaximAuto::chooseGridEasy(int row, int col) {
    int curBiggest = -1;
    int ansRow = row, ansCol = col;
    for (int i = 0; i < squareLength; i++) {
        if (states[row][i] == EMPTY && numbers[row][i] > curBiggest) {
            curBiggest = numbers[row][i];
            ansCol = i;   // row already == curRow
        }
    }
    for (int i = 0; i < squareLength; i++) {
        if (states[i][col] == EMPTY && numbers[i][col] > curBiggest) {
            curBiggest = numbers[i][col];
            ansCol = col;   // resets column chosen
            ansRow = i;
        }
    }
    if (curBiggest == -1) {   // there are no empty grids on same row/col
        for (int i = 0; i < squareLength; i++) {
            for (int j = 0; j < squareLength; j++) {
                if (states[i][j] == EMPTY && numbers[i][j] > curBiggest) {
                    curBiggest = numbers[i][j];
                    ansRow = i;
                    ansCol = j;
                }
            }
        }
    }
    if (curBiggest == -1)
        return -1;
    else
        return ansRow * 100 + ansCol;
}

bool MaximBase::gameOver() const {
    for (int i = 0; i < squareLength; i++) {
        for (int j = 0; j < squareLength; j++) {
            if (states[i][j] == EMPTY) return false;
        }
    }
    return true;
}

void MaximBase::gameEnd() const {  // to add bookmark, use ctrl + shift + num
    cout << "GAME OVER!";
    if (score1 != score2)
        cout << " Player " << (score1 > score2 ? 1 : 2) << " wins!\n";
    else
        cout << " It's a tie!\n";
}

void MaximBase::outputResult(std::ofstream &file) const {
    file << "Player 1 score: " << score1 << ";\tPlayer 2 score: " << score2 <<
         ";\tDifference: " << score1 - score2 << endl;
}

// mode 0: player 1 is hard, player 2 is easy
// mode 1: 2-player; mode 2: one-player easy; mode 3: one-player hard
//void Maxim::play() {
//    if ((mode != 1 && !curPlayer) || mode == 0) {      // the computer plays
//        int coord;
//        if (mode == 2 || (mode == 0 && curPlayer))
//            coord = chooseGridEasy(curRow, curCol);
//        else
//            coord = chooseGridHard(curRow, curCol, 4);
//        curRow = coord / 100; curCol = coord % 100;
//        mark(curRow, curCol);
//        display();
//    } else {
//        int row, col;
//        bool valid = false;
//        while (!valid) {
//            cout << "\nPlayer " << curPlayer+1 << ", enter your next move: ";
//            cin >> row >> col;
//            if (isValid(row, col)) {
//                valid = true;
//            } else {
//                cout << "Invalid input; try again!\n";
//            }
//        }
//        curRow = row; curCol = col;  // assign inputs
//        mark(curRow, curCol);
//        display();
//        cout << endl;
//    }
//    curPlayer = !curPlayer;   // flip turns
//}

/*Point MaximBase::chooseGrid(int row, int col, int iter) {
    int score = 0, curBiggest = -1000;
    Point point(-1, -1);

    for (int i = 0; i < squareLength; i++) {
        if (states[row][i] == EMPTY) {
            score = numbers[row][i] + calcPoints(row, i, iter);
            //cout << row << " " << i << " " << score << endl;
            if (score > curBiggest) {
                curBiggest = score;
                point.col = i;   // row already == curRow
            }
        }
    }
    for (int i = 0; i < squareLength; i++) {
        if (states[i][col] == EMPTY) {
            score = numbers[i][col] + calcPoints(i, col, iter);
            //cout << i << " " << col << " " << score << endl;
            if (score > curBiggest) {
                curBiggest = score;
                point.col = col;   // resets column chosen
                point.row = i;
            }
        }
    }
    if (curBiggest == -1000) {   // there are no empty grids on same row/col
        for (int i = 0; i < squareLength; i++) {
            for (int j = 0; j < squareLength; j++) {
                if (states[i][j] == EMPTY) {
                    score = numbers[i][j] + calcPoints(i, j, iter);
                    //cout << i << " " << j << " " << score << endl;
                    if (score > curBiggest) {
                        curBiggest = score;
                        point.row = i; point.col = j;
                    }
                }
            }
        }
    }
    assert(curBiggest != -1000);
    return point;
}*/

//void Maxim::play() {
//    if ((mode != 1 && !curPlayer) || mode == 0) {      // the computer plays
//        int coord;
//        if (mode == 2 || (mode == 0 && curPlayer))
//            coord = chooseGridEasy(curRow, curCol);
//        else
//            coord = chooseGridHard(curRow, curCol, 4);
//        curRow = coord / 100; curCol = coord % 100;
//        if (mode == 0 && curPlayer)
//            mark2(curRow, curCol);
//        else
//            mark1(curRow, curCol);
//        display();
//    } else {
//        int row, col;
//        bool valid = false;
//        while (!valid) {
//            cout << "\nPlayer " << curPlayer+1 << ", enter your next move: ";
//            cin >> row >> col;
//            if (isValid(row, col)) {
//                valid = true;
//            } else {
//                cout << "Invalid input; try again!\n";
//            }
//        }
//        curRow = row; curCol = col;  // assign inputs
//        if (!curPlayer) {          // current player is 1
//            mark1(curRow, curCol);
//        } else {
//            mark2(curRow, curCol);
//        }
//        display();
//        cout << endl;
//    }
//    curPlayer = !curPlayer;   // flip turns
//}