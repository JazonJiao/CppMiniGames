//
// Written on 2018/5/24.
/******
    Restructured on 2018/10/15. There is a Maxim class that, like other classes in this project,
    is used in main() to start the game. Then there is a MaximBase

    All fields and methods in MaximBase are protected members. This enables child classes to
    access them.
*/

#ifndef MAXIM_MAXIM_H
#define MAXIM_MAXIM_H

#include <iosfwd>
#include <fstream>



const int MAXIMLENGTH = 9;

//struct Point {  // fiksme: I really don't want to put this in a global namespace...
//    int row, col;
//    Point(int r, int c) : row(r), col(c) {}
//};

class Maxim {
public:
    Maxim();
};


class MaximBase {
protected:   //
    MaximBase(int squareLength);

    enum state {
        EMPTY, P1, P2
    };
    int squareLength;         // the side length of player square; can be 3, 5, 7, or 9

    // these static arrays will be partially filled if squareLength is less than 9.
    int numbers[MAXIMLENGTH][MAXIMLENGTH];
    state states[MAXIMLENGTH][MAXIMLENGTH];

    int curRow, curCol;
    bool curPlayer;     // set to 0 if it's Player 1's turn, 1 otherwise
    int score1, score2;
    //int mode;

    void display() const;
    void mark(int row, int col);

    // this is the method that will behave differently for each child class
    // virtual is optional since there is no polymorphism involved
    virtual bool play() = 0; // fixme: try = 0
    void playerChooses();    // used by MaximOnePlayer and MaximTwoPlayers

    void gameEnd() const;  // display endgame results
    void outputResult(std::ofstream &file) const;

    bool isValid(int row, int col); // whether the grid the user enters is valid
    bool gameOver() const;   // returns true if all grids are occupied, i.e. game has finished
};

class MaximAuto : public MaximBase {
protected:
    int difficulty;

    /// note: cannot initialize base class members in child class. Need to call base class ctor.
    MaximAuto(int sqL, int dif) : MaximBase(sqL), difficulty(dif) {}

    void computerChooses();

    // this helper method is used by MaximZeroPlayer and MaximOnePlayer.
    // I originally included it in the base class, but since MaximTwoPlayers doesn't use it,
    // I later considered this bad style.
    // Now the inheritance structure is a little more complicated due to an intermediate
    // class between base and 0-player & 1-player classes.
    int chooseGrid(int row, int col, int iter);

    // helper method used by chooseGrid
    int calcPoints(int row, int col, int iter);

    // helper method used by calcPoints. Also can be used as a standalone method for 1-ply
    int chooseGridEasy(int row, int col);
};

class MaximZeroPlayer : public MaximAuto {
public:
    /// note: cannot initialize base class members in child class. Need to call base class ctor.
    MaximZeroPlayer(int sqL, int dif) : MaximAuto(sqL, dif) {
        while (play());
        gameEnd();
    }

    virtual bool play();
};

// all protected fields/methods should be inherited
class MaximOnePlayer : public MaximAuto {
public:
    MaximOnePlayer(int sqL, int dif) : MaximAuto(sqL, dif) {
        while (play());
        gameEnd();
    }

    virtual bool play();
};

class MaximTwoPlayers : public MaximBase {
public:
    MaximTwoPlayers(int sqL) : MaximBase(sqL) {
        while (play());
        gameEnd();
    }
    virtual bool play();
};



#endif //MAXIM_MAXIM_H
