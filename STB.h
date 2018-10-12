// implementation of the game Shut The Box
// by Jazon Jiao, 2018.09.30
// https://brilliant.org/practice/shut-the-box-endgame-strategy/?p=1
//

#ifndef GAMES_STB_H
#define GAMES_STB_H

//#include <cstdint>


const int NUM_TILES = 10; // use one-based indexing; actually only has 9 boxes

class STB {
public:
    STB();

private:
    bool tiles[NUM_TILES]; // false if shut, true if open
    // (it's so hard to decide if shut is represented by true or false, but true for open is better
    // since a lot of operations are based on open boxes)
    bool canRoll1;   // whether player can roll only 1 dice

    int roll1();
    int roll2();
    bool setRollOne();
    bool lost(int sum);
    bool won();

    int roll();
    void shut();

    void intro();
    bool play();
    void display();
    void gameover();
    void gamewon();


};


#endif //GAMES_STB_H
