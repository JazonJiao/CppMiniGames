//
// Created by 焦玉华 on 2018/5/29.
//

#include <cstdlib>
#include <iostream>
#include <cassert>
#include "Gomoku.h"

using namespace std;
typedef int num; // in case we need more than 10-size grid

Gomoku::Gomoku() : curRow(-1), curCol(-1), curPlayer(false) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            states[i][j] = EMPTY;
    cout << "Welcome to Gomoku! The first person to get " << STEP << " pieces in line wins. \n\n";
    while (play());
}

void Gomoku::mark(int row, int col) {
    states[row][col] = curPlayer ? P2 : P1;
}

bool Gomoku::play() {
    string str;
    display();
    cout << "Player " << curPlayer+1 << ", enter next move: ";
    cin >> curRow >> curCol;
    getline(cin, str);  // consume existing line
    while (not isValid(curRow, curCol)) {
        cout << "Invalid input. Try again: ";
        cin >> curRow >> curCol;
        getline(cin, str);
    }
    mark(curRow, curCol);

    if (won(curRow, curCol)) {
        display();
        cout << "Player " << curPlayer+1 << ", congrats! You won! " << endl;
        return false;
    }
    curPlayer = not curPlayer;
    return true;
}

bool Gomoku::isValid(int row, int col) {
    return row > -1 && row < SIZE && col > -1 && col < SIZE && states[row][col] == EMPTY;
}

bool Gomoku::won(int row, int col) {
    state player = curPlayer ? P2 : P1;
    int stepL = 0, stepUL = 0, stepU = 0, stepUR = 0, stepR = 0, stepDR = 0, stepD = 0, stepDL = 0;

    assert(states[row][col] == player);

    for (int j = 1; j <= STEP; j++) {   // checks left
        if (col - j < 0 || states[row][col - j] != player)
            break;
        else
            stepL++;
    }
    if (stepL + 1 == STEP)
        return true;
    for (int j = 1; j <= STEP - stepL; j++) {  // checks right
        if (col + j >= SIZE || states[row][col + j] != player)
            break;
        else
            stepR++;
    }
    if (stepL + stepR + 1 == STEP)
        return true;

    for (int i = 1; i <= STEP; i++) {  // checks up-left diagonal
        if (col - i < 0 || row - i < 0 || states[row - i][col - i] != player)
            break;
        else
            stepUL++;
    }
    if (stepUL + 1 == STEP)
        return true;
    for (int i = 1; i <= STEP - stepUL; i++) {
        if (col + i >= SIZE || row + i >= SIZE || states[col + i][row + i] != player)
            break;
        else
            stepDR++;
    }
    if (stepUL + stepDR + 1 == STEP)
        return true;

    for (int i = 1; i <= STEP; i++) {
        if (row - i < 0 || states[row - i][col] != player)
            break;
        else
            stepU++;
    }
    if (stepU + 1 == STEP)
        return true;
    for (int i = 1; i <= STEP - stepU; i++) {
        if (row + i >= SIZE || states[row + i][col] != player)
            break;
        else
            stepD++;
    }
    if (stepU + stepD + 1 == STEP)
        return true;

    for (int i = 1; i <= STEP; i++) {
        if (row - i < 0 || col + i >= SIZE || states[row - i][col + i] != player)
            break;
        else
            stepUR++;
    }
    if (stepUR + 1 == STEP)
        return true;
    for (int i = 1; i <= STEP - stepUR; i++) {
        if (row + i >= SIZE || col - i < 0 || states[row + i][col - i] != player)
            break;
        else
            stepDL++;
    }
    if (stepUR + stepDL + 1 == STEP)
        return true;

    cout << stepL << stepUL << stepU << stepUR << stepR << stepDR << stepD << stepDL << endl;
    return false;
}

void Gomoku::display() const {
    cout << "\n ";
    for (num i = 0; i < SIZE; i++)
        cout << "|" << i;
    cout << "|" << endl;

    for (num j = SIZE - 1; j >= 0; j--) {
        cout << j;
        for (num i = 0; i < SIZE; i++) {
            if (states[i][j] == EMPTY)
                cout << "| ";
            else if (states[i][j] == P1)
                cout << "|O";
            else
                cout << "|X";
        }
        cout << "|" << j << endl;
    }
    cout << " ";
    for (num i = 0; i < SIZE; i++)
        cout << "|" << i;
    cout << "|" << endl;
}