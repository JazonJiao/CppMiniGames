//
// Created by 焦玉华 on 2018-10-18.
//

#include "G2048.h"
#include <iostream>
#include <random>
#include <ctime>


#define random(a, b) (rand() % (b-a+1) + a)

using std::cout;
using std::cin;

G2048::G2048() : numEmpty(LEN_2048 * LEN_2048), score(0) {
    for (int i = 0; i < LEN_2048; ++i)
        for (int j = 0; j < LEN_2048; ++j)
            grid[i][j] = 0;
    generate();
    while (play());
}

void G2048::display() const {
    for (int i = 0; i < LEN_2048; ++i)
        displayLine(i);
    cout << "+----+----+----+----+\n";
}

void G2048::displayLine(int line) const {
    cout << "+----+----+----+----+\n"
            "|";
    for (int i = 0; i < LEN_2048; ++i) {
        int val = grid[line][i];
        if (val == 0)
            cout << "    |";
        else if (val < 10)
            cout << "  " << val << " |";
        else if (val < 100)
            cout << ' ' << val << " |";
        else if (val < 1000)
            cout << ' ' << val << '|';
        else if (val < 10000)
            cout << val << '|';
        else
            cout << (val / 1000) << "K+";  // just for fun
    }
    cout << '\n';
}

bool G2048::play() {
    generate();
    display();

    char choice;
    cout << "Enter your move (W/A/S/D): ";
    cin >> choice;
    choice = (char) std::tolower(choice);
    switch (choice) {
        case 'w': {
            moveU(); // fixme: break?
            break;
        } case 'a': {
            //moveL();
        }
        case 's': {
            //moveD();
        }
        case 'd': {
            //moveR();
        }
        default: {
            cout << "Invalid input. Try again: ";
            return true;
        }
    }

    return true;
}

void G2048::generate() {
    // first construct an array that stores the empty grids.
    // coordinate = row * 4 + col.
    int* emptys = new int[numEmpty];
    int k = 0;
    for (int i = 0; i < LEN_2048; ++i) {
        for (int j = 0; j < LEN_2048; ++j)
        if (grid[i][j] == 0)
            emptys[k++] = i * LEN_2048 + j;
    }

    // copied from sample code and internet
    /// Error log: Note that these objects must be declared as static.
    /// Otherwise, due to the speed of program, the random generator object will be constructed
    /// multiple times in almost no seconds,
    /// causing it to generate the same number / very close numbers.
    static std::mt19937 rng((unsigned) time(nullptr));

    /// Error log: the right end is inclusive! Need to - 1!
    static std::uniform_int_distribution<int> uid(0, numEmpty - 1);

    int index = uid(rng);
    //cout << index << std::endl;
    int coord = emptys[index];
    // determine if the grid will be 2 or 4
    double is4 = rng() / (double) UINT32_MAX;
    //cout << is4 << std::endl;
    if (is4 < PROB_4) {
        grid[coord / 4][coord % 4] = 4;
    } else {
        grid[coord / 4][coord % 4] = 2;
    }
    /// Error log: don't forget this!
    delete [] emptys;
    --numEmpty;
}

void G2048::moveU() {
    // scan each column from left to right
    for (int col = 0; col < LEN_2048; ++col) {
        // shift the column upwards to fill empty grids
        for (int row = 1; row < LEN_2048; ++row) {
            int i = row;
            while (i > 0 and grid[i - 1][col] == 0) {
                std::swap(grid[i - 1][col], grid[i][col]);
                i--;
            }
        }

        // scan each row from top to down
        for (int row = 0; row < LEN_2048 - 1; ++row) {
            // check for adjacent grids, if the same then merge
            if (grid[row][col] == grid[row + 1][col] != 0) {
                grid[row][col] *= 2;
                grid[row + 1][col] = 0;
                score += grid[row][col];
            }
        }
        // shift the column upwards to fill empty grids
        for (int row = 1; row < LEN_2048; ++row) {
            int i = row;
            while (i > 0 and grid[i - 1][col] == 0) {
                std::swap(grid[i - 1][col], grid[i][col]);
                i--;
            }
        }
    }
}