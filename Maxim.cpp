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

using namespace std;

// GAMEPLAY
// the game is played on a 7 * 7 grid, each grid having a number initialized randomly from 0 to 9.
// the central grid is initialized to 0, and Player 1 starts with it marking an "O" on it.
// then Player 2 chooses a grid that's either on the same row or the same column as
// the grid just chosen, adds the number on that grid to his/her score, and marking an "X" on it.
// Then the two players take turns to choose grids until all grids are chosen.
// The player with a higher total score wins.
// if one-player mode is chosen, computer is Player 1.
Maxim::Maxim() : curRow(LENGTH/2), curCol(LENGTH/2), curPlayer(1), score1(0), score2(0) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            numbers[i][j] = random(0, 9);
        }
    }
    //numbers[curRow][curCol] = 0;  // set central grid to be 0

    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            states[i][j] = EMPTY;
        }
    }
    states[curRow][curCol] = P1;

    cout << "Welcome to Maxim! Enter Mode--\n"
            "0, Zero-player; 1, Two Players; 2, One-player Easy; 3, One-player Hard: " << endl;
    cin >> mode;
    //mode = 0;
    assert(mode >= 0 && mode < 4);
    while (not finished()) {
        display();
        play();
    }

}

// when displaying, to use Cartesian coordinates,
// the row and column are swapped and flipped
void Maxim::display() const {
    cout << "^ y-axis" << endl;
    for (int i = LENGTH - 1; i >= 0; i--) {
        for (int j = 0; j < LENGTH; j++) {
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
    cout << "Current Grid: " << curRow << "," << curCol << "; Current Player: " << curPlayer+1;
    cout << "\nScores--Player 1: " << score1 << "; Player 2: " << score2 << endl;
}

void Maxim::mark(int row, int col) {
    states[row][col] = curPlayer ? P2 : P1;
    if ( ! curPlayer) score1 += numbers[row][col];
    else score2 += numbers[row][col];
}

//void Maxim::mark2(int row, int col) {
//    states[row][col] = P2;
//    score2 += numbers[row][col];
//}

// mode 0: player 1 is hard, player 2 is easy
// mode 1: 2-player; mode 2: one-player easy; mode 3: one-player hard
void Maxim::play() {
    if ((mode != 1 && !curPlayer) || mode == 0) {      // the computer plays
        int coord;
        if (mode == 2 || (mode == 0 && curPlayer))
            coord = chooseGridEasy(curRow, curCol);
        else
            coord = chooseGridHard(curRow, curCol, 4);
        curRow = coord / 100; curCol = coord % 100;
        mark(curRow, curCol);
        display();
    } else {
        int row, col;
        bool valid = false;
        while (!valid) {
            cout << "\nPlayer " << curPlayer+1 << ", enter your next move: ";
            cin >> row >> col;
            if (isValid(row, col)) {
                valid = true;
            } else {
                cout << "Invalid input; try again!\n";
            }
        }
        curRow = row; curCol = col;  // assign inputs
        mark(curRow, curCol);
        display();
        cout << endl;
    }
    curPlayer = !curPlayer;   // flip turns
}

// tests whether the grid coordinates the user enters is valid
// the user's grid should be on the same row or column as the current grid
// if all grids on the row or column are occupied, the user can choose from any remaining grid
bool Maxim::isValid(int row, int col) {
    if (row >= LENGTH || row < 0 || col >= LENGTH || col < 0)
        return false;
    bool hasEmpty = false;              // whether there are empty grids on the same row/col
    for (int i = 0; i < LENGTH; i++) {
        if (states[curRow][i] == EMPTY) hasEmpty = true;
    }
    for (int i = 0; i < LENGTH; i++) {
        if (states[i][curCol] == EMPTY) hasEmpty = true;
    }
    if (hasEmpty) {
        return (row == curRow || col == curCol) && states[row][col] == EMPTY;
    } else {
        return states[row][col] == EMPTY;
    }
}

// easy algorithm: one-ply (search for the largest number available)
// if game is over, return -1 (should not happen);
// else returns a three-digit number; the first digit is row, last is col
int Maxim::chooseGridEasy(int row, int col) {
    int curBiggest = -1;
    int ansRow = row, ansCol = col;
    for (int i = 0; i < LENGTH; i++) {
        if (states[row][i] == EMPTY && numbers[row][i] > curBiggest) {
            curBiggest = numbers[row][i];
            ansCol = i;   // row already == curRow
        }
    }
    for (int i = 0; i < LENGTH; i++) {
        if (states[i][col] == EMPTY && numbers[i][col] > curBiggest) {
            curBiggest = numbers[i][col];
            ansCol = col;   // resets column chosen
            ansRow = i;
        }
    }
    if (curBiggest == -1) {   // there are no empty grids on same row/col
        for (int i = 0; i < LENGTH; i++) {
            for (int j = 0; j < LENGTH; j++) {
                if (states[i][j] == EMPTY && numbers[i][j] > curBiggest) {
                    curBiggest = numbers[i][j];
                    ansRow = i; ansCol = j;
                }
            }
        }
    }
    if (curBiggest == -1)
        return -1;
    else
        return ansRow * 100 + ansCol;
}

// helper recursive method for chooseGridHard()
// returns the total net points after a certain iteration of optimized plays,
// not including the point of the first grid that's passed from outside
int Maxim::calcPoints(int row, int col, int iter) {
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
int Maxim::chooseGridHard(int row, int col, int iter) {   // modified from chooseGridEasy()
    int score = 0, curBiggest = -1000;
    int ansRow = row, ansCol = col;

    for (int i = 0; i < LENGTH; i++) {
        if (states[row][i] == EMPTY) {
            score = numbers[row][i] + calcPoints(row, i, iter);
            //cout << row << " " << i << " " << score << endl;
            if (score > curBiggest) {
                curBiggest = score;
                ansCol = i;   // row already == curRow
            }
        }
    }
    for (int i = 0; i < LENGTH; i++) {
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
    if (curBiggest == -1000) {   // there are no empty grids on same row/col
        for (int i = 0; i < LENGTH; i++) {
            for (int j = 0; j < LENGTH; j++) {
                if (states[i][j] == EMPTY) {
                    score = numbers[i][j] + calcPoints(i, j, iter);
                    //cout << i << " " << j << " " << score << endl;
                    if (score > curBiggest) {
                        curBiggest = score;
                        ansRow = i; ansCol = j;
                    }
                }
            }
        }
    }
    assert(curBiggest != -1000);
    return ansRow * 100 + ansCol;
}

bool Maxim::finished() const {
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < LENGTH; j++) {
            if (states[i][j] == EMPTY) return false;
        }
    }
    return true;
}

void Maxim::showResult() const {   // to add bookmark, use ctrl + shift + num
    cout << "GAME OVER!";
    if (score1 != score2) cout << " Player " << (score1 > score2 ? 1 : 2) << " wins!\n";
    else cout << " It's a tie!\n";
}

void Maxim::outputResult(ofstream & file) {
    file << "Player 1 score: " << score1 << ";\tPlayer 2 score: " << score2 <<
           ";\tDifference: " << score1 - score2 << endl;
}

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