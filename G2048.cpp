//
// Created by 焦玉华 on 2018-10-18.
//

#include "G2048.h"
#include <iostream>
#include <random>
#include <ctime>
//#include <memory>

using std::cout;
using std::cin;

G2048::G2048() : numEmpty(LEN_2048 * LEN_2048), addedScore(0), score(0),
                 head(nullptr), rounds(0), notWon(true) {
    for (int i = 0; i < LEN_2048; ++i) {
        for (int j = 0; j < LEN_2048; ++j)
            grid[i][j] = 0;
    }
    cout << "Welcome to 2048!\n"
            "Type W/A/S/D to move up/down/left/right;\n"
            "or enter R to reverse one round.\n";

    generate();
    generate();
    display();
    logProgress();

    while (play());
}

G2048::~G2048() {
    Node2048* tmp;
    while (head != nullptr) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

void G2048::display() const {
    for (int i = 0; i < LEN_2048; ++i)
        displayLine(i);
    cout << "+----+----+----+----+\n";
    cout << "Your score: + " << addedScore << " = " << score << std::endl;
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
    // get user's move
    char choice;
    cout << "\nEnter your move (W/A/S/D/R): ";
    cin >> choice;

    // if no grid moves or input char is invalid,
    // we return true to avoid generating new number and
    // continue the while loop in the ctor.
    addedScore = 0;
    if (not move((char) std::tolower(choice))) {
        cout << "Invalid move. Try again: \n";
        return true;
    }
    // user reverses a round, and this reverse is valid
    if (choice == 'r' or choice == 'R') {
        display();
        return true;
    }
    score += addedScore;
    ++rounds;

    generate();
    display();
    logProgress();

    if (notWon) {
        if (won()) {
            return gameWon();
        }
    }
    if (lost()) {
        return gameLost();
    }
    return true;
}

void G2048::generate() {
    // first construct a dynamic array that stores the empty grids' coordinates.
    // coordinate = row * 4 + col, ranging from 0 to 15.
    int *emptys(new int[numEmpty]);
    int k = 0;
    for (int i = 0; i < LEN_2048; ++i) {
        for (int j = 0; j < LEN_2048; ++j)
            if (grid[i][j] == 0)
                emptys[k++] = i * LEN_2048 + j;
    }

    // copied from sample code and internet
    /// Error log: Note that this object must be declared as static.
    /// Otherwise, due to the speed of program, the random generator will be constructed
    /// multiple times in almost no seconds,
    /// causing it to generate the same number / very close numbers.
    static std::mt19937 rng((unsigned) time(nullptr));

    // rng() returns an int less than UINT32_MAX. Index ranges from 0 to numEmpty - 1.
    int index = rng() % numEmpty;
    int coord = emptys[index];

    // determine if the grid will be 2 or 4
    grid[coord / 4][coord % 4] = 2;
    if (PROB_4 > 0) {
        double is4 = rng() / (double) UINT32_MAX;
        if (is4 < PROB_4) {
            grid[coord / 4][coord % 4] = 4;
        }
    }
    --numEmpty;
    /// Error log: don't forget this!
    delete [] emptys;
}

bool G2048::move(char choice) {
    bool moved, merged;
    switch (choice) {
        case 'w': {
            moved = moveU();
            merged = mergeU();
            moveU();
            return (moved or merged);
        }
        case 'a': {
            moved = moveL();
            merged = mergeL();
            moveL();
            return (moved or merged);
        }
        case 's': {
            moved = moveD();
            merged = mergeD();
            moveD();
            return (moved or merged);
        }
        case 'd': {
            moved = moveR();
            merged = mergeR();
            moveR();
            return (moved or merged);
        }
        case 'r': {
            return reverse();
        }
        default: {
            return false;
        }
    }
}

bool G2048::moveU() {
    bool moved = false;
    // scan each column from left to right
    for (int col = 0; col < LEN_2048; ++col) {
        // shift the column upwards to fill empty grids
        for (int row = 1; row < LEN_2048; ++row) {
            int i = row;
            while (i > 0 and grid[i][col] != 0 and grid[i - 1][col] == 0) {
                std::swap(grid[i][col], grid[i - 1][col]);
                --i;
                moved = true;
            }
        }
    }
    return moved;
}

bool G2048::moveD() {
    bool moved = false;
    // scan each column from left to right
    for (int col = 0; col < LEN_2048; ++col) {
        for (int row = LEN_2048 - 1; row > 0; --row) {
            int i = row;
            while (i < LEN_2048 and grid[i][col] == 0 and grid[i - 1][col] != 0) {
                std::swap(grid[i][col], grid[i - 1][col]);
                ++i;
                moved = true;
            }
        }
    }
    return moved;
}

bool G2048::moveL() {
    bool moved = false;
    for (int row = 0; row < LEN_2048; ++row) {
        for (int col = 1; col < LEN_2048; ++col) {
            int i = col;
            while (i > 0 and grid[row][i] != 0 and grid[row][i - 1] == 0) {
                std::swap(grid[row][i], grid[row][i - 1]);
                --i;
                moved = true;
            }
        }
    }
    return moved;

}

bool G2048::moveR() {
    bool moved = false;
    // scan each row from top to bottom
    for (int row = 0; row < LEN_2048; ++row) {
        // scan each column from right to left
        for (int col = LEN_2048 - 1; col > 0; --col) {
            int i = col;
            while (i < LEN_2048 and grid[row][i] == 0 and grid[row][i - 1] != 0) {
                std::swap(grid[row][i], grid[row][i - 1]);
                ++i;
                moved = true;
            }
        }
    }
    return moved;
}


bool G2048::mergeU() {
    bool merged = false;
    // scan each column from left to right
    for (int col = 0; col < LEN_2048; ++col) {
        // scan each row from top to down
        for (int row = 0; row < LEN_2048 - 1; ++row) {
            // check for adjacent grids, if the same then merge
            if (grid[row][col] != 0 and grid[row][col] == grid[row + 1][col]) {
                // expression evaluates from right to left;
                // grid is updated, then score is updated
                addedScore += grid[row][col] *= 2;
                grid[row + 1][col] = 0;
                ++numEmpty;  // CAVEAT: there is one more empty grid now
                merged = true;
            }
        }
    }
    return merged;
}

bool G2048::mergeD() {
    bool merged = false;
    for (int col = 0; col < LEN_2048; ++col) {
        // scan from bottom to top
        for (int row = LEN_2048 - 1; row > 0; --row) {
            if (grid[row][col] != 0 and grid[row][col] == grid[row - 1][col]) {
                addedScore += grid[row][col] *= 2;  // update down grid
                grid[row - 1][col] = 0;    // reset up grid
                ++numEmpty;
                merged = true;
            }
        }
    }
    return merged;
}


bool G2048::mergeL() {
    bool merged = false;
    for (int row = 0; row < LEN_2048; ++row) {
        for (int col = 0; col < LEN_2048 - 1; ++col) {
            /// Error log: set the righter grid to 2 times the value and lefter grid to 0
            /// should do the opposite to avoid merging, say, 2 2 4 into one 8
            if (grid[row][col] != 0 and grid[row][col] == grid[row][col + 1]) {
                addedScore += grid[row][col] *= 2;
                grid[row][col + 1] = 0;
                ++numEmpty;
                merged = true;
            }
        }
    }
    return merged;
}

bool G2048::mergeR() {
    bool merged = false;
    for (int row = 0; row < LEN_2048; ++row) {
        for (int col = LEN_2048 - 1; col > 0; --col) { /// Error log: forgit to --
            // scan from right to left
            if (grid[row][col] != 0 and grid[row][col] == grid[row][col - 1]) {
                addedScore += grid[row][col] *= 2;   // update right grid
                grid[row][col - 1] = 0;  // set left grid to 0
                ++numEmpty;
                merged = true;
            }
        }
    }
    return merged;
}


bool G2048::lost() const {
    // return false if there is an empty grid
    for (int i = 0; i < LEN_2048; ++i) {
        for (int j = 0; j < LEN_2048; ++j)
            if (grid[i][j] == 0)
                return false;
    }
    // scan each row for identical adjacent values
    for (int row = 0; row < LEN_2048; ++row) {
        for (int col = 0; col < LEN_2048 - 1; ++col) {
            if (grid[row][col] == grid[row][col + 1])
                return false;
        }
    }
    // scan each column for identical adjacent values
    for (int col = 0; col < LEN_2048; ++col) {
        for (int row = 0; row < LEN_2048 - 1; ++row) {
            if (grid[row][col] == grid[row + 1][col])
                return false;
        }
    }
    return true;
}

bool G2048::won() {
    for (int i = 0; i < LEN_2048; ++i) {
        for (int j = 0; j < LEN_2048; ++j) {
            if (grid[i][j] == 2048) {
                notWon = false;
                return true;
            }
        }
    }
    return false;
}

bool G2048::gameLost() {
    cout << "Sorry, you lost! \n"
            "Do you want to revert one step? (Y/N): ";
    char choice;
    cin >> choice;
    if (choice == 'n' or choice == 'N') {
        return false;
    }
    reverse();
    display();
    return true;
}

bool G2048::gameWon() const {
    cout << "Congrats, you won! \n"
            "Do you want to keep playing? (Y/N): ";
    char choice;
    cin >> choice;
    return (choice == 'y' or choice == 'Y');
}

void G2048::logProgress() {
    auto tmp = new Node2048;
    tmp->nEmpty = numEmpty;
    for (int i = 0; i < LEN_2048; ++i)
        for (int j = 0; j < LEN_2048; ++j)
            tmp->grids[i][j] = grid[i][j];
    tmp->next = head;
    head = tmp;
}

bool G2048::reverse() {
    // check if it's the first round; head should never be null when reverse() is called
    if (head->next == nullptr)
        return false;
    // delete the first node
    auto tmp = head;
    head = head->next;
    delete tmp;
    // don't forget to update rounds
    --rounds;
    // reassign current grid to the previous grid
    numEmpty = head->nEmpty;
    for (int i = 0; i < LEN_2048; ++i)
        for (int j = 0; j < LEN_2048; ++j)
            grid[i][j] = head->grids[i][j];
    return true;
}


