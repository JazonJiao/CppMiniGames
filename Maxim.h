//
// Created by 焦玉华 on 2018/5/24.
//

#ifndef MAXIM_MAXIM_H
#define MAXIM_MAXIM_H

#include <iosfwd>
#include <fstream>

const int LENGTH = 5;     // the side length of player square; can be 3, 5, 7, or 9

class Maxim {
private:
    enum state {
        EMPTY, P1, P2
    };
    int numbers[LENGTH][LENGTH];
    state states[LENGTH][LENGTH];
    int curRow, curCol;
    bool curPlayer;     // set to 0 if it's Player 1's turn, 1 otherwise
    int score1, score2;
    int mode;

public:
    Maxim();

    void display() const;                 // display the board
    void mark(int row, int col);   // mark a - for player 1
    //void mark2(int row, int col);   // mark a space for player 2
    void play();                 // plays one round
    //void oneplayer();               // plays one round in one player mode; computer is player 1
    void showResult() const;

    void outputResult(std::ofstream &file);

    int chooseGridEasy(int row, int col);      // computer chooses the next move
    int chooseGridHard(int row, int col, int iter);

    int calcPoints(int row, int col, int iter);

    bool isValid(int row, int col); // whether the grid the user enters is valid
    bool finished() const;


};


#endif //MAXIM_MAXIM_H
