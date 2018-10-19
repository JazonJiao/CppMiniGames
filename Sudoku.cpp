//
// Created by 焦玉华 on 2018-10-17.
//

#include <fstream>
#include <cassert>
#include "Sudoku.h"

using std::cin;
using std::cout;

Sudoku::Sudoku() {
    readGrid();
    cout << "\n---Welcome to Mine Sweeper!--- \n"
            "For each round, please enter 3 numbers:\n"
            "#1 for row number, #2 for columnnumber,\n"
            "#3 for one of 1~9 you want to put in,\n"
            "   or 0 if you want to erase it.\n"
            "Good luck!\n\n";
    while (play());
    display();
    cout << "\nCongrats! You solved the puzzle!\n";
}

void Sudoku::readGrid() {
    int tmp;
    std::ifstream fin(FILENAME.c_str());
    assert(fin);

    for (int i = 0; i < SUDOKULEN; ++i) {
        for (int j = 0; j < SUDOKULEN; ++j) {
            fin >> tmp;
            grid[i][j] = tmp;
            if (tmp == 0)
                states[i][j] = EMPTY;
            else
                states[i][j] = GIVEN;
        }
    }
}

void Sudoku::display() const {
    // print the beginning
    cout << "\n    ";
    for (int i = 0; i < SUDOKULEN; ++i)
        cout << i + 1 << ((i == 2 or i == 5) ? " | " : "  ");

    cout << "\n+--";
    for (int i = 0; i < SUDOKULEN; ++i)
        cout << ((i == 2 or i == 5) ? "+-+-" : "+-+");
    cout << "--+\n";

    // print the middle bulk
    for (int j = 0; j < SUDOKULEN; ++j) {
        cout << j + 1 << " >";
        for (int i = 0; i < SUDOKULEN; ++i) {
            if (states[j][i] == GIVEN)
                cout << ' ' << grid[j][i] << ' ';
            else if (states[j][i] == EMPTY)
                cout << "( )";
            else if (states[j][i] == FILLED)
                cout << '(' << grid[j][i] << ')';
            if (i == 2 or i == 5)
                cout << ':';
        }
        cout << "< " << j + 1 << '\n';
        if (j == 2 or j == 5) {
            for (int k = 0; k < 18; ++k)
                cout << "- ";
            cout << '\n';
        }
    }

    cout << "+--";
    for (int i = 0; i < SUDOKULEN; ++i)
        cout << ((i == 2 or i == 5) ? "+-+-" : "+-+");
    cout << "--+\n";

    cout << "    ";
    for (int i = 0; i < SUDOKULEN; ++i)
        cout << i + 1 << ((i == 2 or i == 5) ? " | " : "  ");
    cout << '\n';
}

bool Sudoku::isValid(int row, int col, int n) const {
    // check if row, col, n are valid numbers
    if (row < 0 or row > 8 or col < 0 or col > 8 or n < 0 or n > 9)
        return false;

    if (states[row][col] == GIVEN)
        return false;

    if (n == 0)  // there may be many empty blocks...
        return true;

    for (int j = 0; j < SUDOKULEN; ++j) {
        //cout << grid[row][j];
        if (grid[row][j] == n)
            return false;
    }

    for (int i = 0; i < SUDOKULEN; ++i) {
        //cout << grid[i][col];
        if (grid[i][col] == n)
            return false;
    }

    // check local 3x3 grid
    int x = row / 3, y = col / 3;
    for (int i = x * 3; i < x * 3 + 3; ++i) {
        for (int j = y * 3; j < y * 3 + 3; ++j) {
            if (grid[i][j] == n)
                return false;
        }
    }
    return true;
}

void Sudoku::mark(int row, int col, int n) {
    grid[row][col] = n;
    if (n == 0) {
        states[row][col] = EMPTY;
    } else {
        states[row][col] = FILLED;
    }
}

bool Sudoku::play() {
    display();

    std::string str;
    int x, y, n;
    cout << "\nPlease enter your next move: ";
    cin >> x >> y >> n;
    --x;
    --y;
    std::getline(std::cin, str); // consume current line
    if (not isValid(x, y, n)) {
        cout << "Invalid input. Try again: ";
        return true;
    }
    mark(x, y, n);

    return not solved();
}

bool Sudoku::solved() const {
    for (int i = 0; i < SUDOKULEN; ++i) {
        for (int j = 0; j < SUDOKULEN; ++j) {
            if (states[i][j] == EMPTY)
                return false;
        }
    }
    return true;
}