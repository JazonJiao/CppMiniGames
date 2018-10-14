//
// Created by 焦玉华 on 2018/5/29.
//

#ifndef MAXIM_GOMOKU_H
#define MAXIM_GOMOKU_H

#include "Maxim.h"

const int SIZE = 10; // size of the n by n board
const int STEP = 5;  // the number of aligned pieces needed for winning


class Gomoku {
private:
    enum state {
        EMPTY, P1, P2
    };
    state states[SIZE][SIZE];
    int curRow, curCol;
    bool curPlayer;

    void mark(int row, int col);
    bool isValid(int row, int col);
    bool won(int row, int col);  // checks if conditions for winning are satisfied around a point

public:
    Gomoku();

    bool play();            // returns true if one side has won
    void display() const;
};


#endif //MAXIM_GOMOKU_H
