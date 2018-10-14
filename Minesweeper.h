//
// Created by 焦玉华 on 2018-07-21.
//

#ifndef MAXIM_MINESWEEPER_H
#define MAXIM_MINESWEEPER_H

#include <iostream>
#include <ctime>
#include <cassert>
#include <fstream>


class Minesweeper {
public:
    Minesweeper();

private:
    enum cond {UNTOUCHED, REVEALED, FLAGGED, QUESTION}; // I can't use state here. Damn...
    // fixed: moved all enum and global constants to private namespace on 10/13/2018

    static const int W = 10;  // fixme: needs to be static else array can't be initialized
    static const int H = 10;  // fixme: so these variables are essentially still in global namespace
    static const int HIGHSCORERS = 8;

    const double difficulty = 0.15;  // probability that a given grid is a mine
    const std::string FILENAME = "MinesweeperHS.txt";
    //enum content {EMPTY, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, MINE};
    const int MINE = -1;
    typedef int content;  // == -1 for mines, 0~8 for number of adjacent mines


    content contents[W][H];
    cond states[W][H];
    int numMines;
    time_t start, end;

    std::string names[HIGHSCORERS];
    double scores[HIGHSCORERS];

    void updatecontent(int x, int y);
    void flag(int x, int y);
    void reveal(int x, int y);

    bool valid(int x, int y);
    bool checkwon();
    void readHS();
    void writeHS(double newHS);
    void displayHS();

    void display();
    bool play();
    void gamewon();
    void gameover();
};


#endif //MAXIM_MINESWEEPER_H
