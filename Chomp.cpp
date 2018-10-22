//    0 1 2 3 4 5 6 7 8 9 101112131415
//    ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
//1 < X O O O O O
//2 < O O O O
//    ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
//    1 2 3 4 5 6 7 8 9 1011121314

#include "Chomp.h"
#include <iostream>
#include <cassert>

using std::cout;
using std::cin;

Chomp::Chomp() : curPlayer(false) {
    int col;
    cout << "Welcome to chomp! \n"
            "Enter number of rows (max. " << MAX_ROW << "): ";
    cin >> height;
    cout << "Enter number of columns (max. " << MAX_COL << "): ";
    cin >> col;
    assert(height > 0 and height <= MAX_ROW and col > 0 and col <= MAX_COL);

    // construct grid
    arr = new int[height];
    for (int i = 0; i < height; ++i) {
        arr[i] = col; // when displaying, will show indices from 0 to col - 1
    }

    display();
    while (play());
}

Chomp::~Chomp() {
    delete arr;
}

bool Chomp::valid(int row, int col) const {
    if (row < 0 or row >= height or col < 0)
        return false;
    if (row == 0 and col == 0)
        return false;
    return arr[row] >= col;
}

void Chomp::display() const {
    // print the beginning indexes
    cout << "     ";
    int width = arr[0];
    for (int k = 0; k < width; ++k) {
        cout << k;
        if (k < 10)
            cout << ' ';
    }
    cout << "\n    ";
    for (int k = 0; k < width; ++k)
        cout << " ^";
    cout << '\n';

    // print the rest
    int i = 0;
    // height might be greater than number of rows left after chopping
    while (i < height and arr[i] != 0) {
        if (i < 10)
            cout << " ";
        cout << i << " < ";
        cout << (i == 0 ? 'X' : 'O');
        for (int j = 1; j < arr[i]; ++j)
            cout << " O";
        cout << '\n';
        ++i;
    }
}

bool Chomp::play() {
    int row, col;
    cout << "\nPlayer " << (curPlayer + 1) << ", enter your move: ";
    cin >> row >> col;
    if (not valid(row, col)) {
        cout << "Invalid input. Try again: ";
        return true;
    }

    chop(row, col);
    display();

    if (lost()) {
        cout << "\nPlayer " << (curPlayer + 1) << ", congrats--you won! \n";
        return false;
    }
    curPlayer = not curPlayer;
    return true;
}


void Chomp::chop(int row, int col) {
    for (int i = row; i < height and arr[i] > col; ++i) {
        arr[i] = col;
    }
}

bool Chomp::lost() const {
    if (height == 1) {
        return arr[0] == 1;
    } else {
        return arr[0] == 1 and arr[1] == 0;
    }
}