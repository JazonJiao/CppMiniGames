// implementation of the game Shut The Box
// by Jazon Jiao, 2018.09.30
// https://brilliant.org/practice/shut-the-box-endgame-strategy/?p=1
//

#include "STB.h"
#include <random>
#include <ctime>
#include <iostream>

#define random(a, b) (rand() % (b-a+1) + a)

using namespace std;

// GAMEPLAY
// Each turn, you the player rolls two dice, and add their sum. Then you shut one or more boxes
// as long as the sum of the number on these boxes equal the sum of two dice.
// For example, if you roll an 8, the following combinations of tiles could be shut:
// (8), (7,1), (6,2), (5,3), (4,3,1), (5,2,1).
// If there's no way you can shut any boxes, then you lose. When all boxes are shut, you win.
// If the tiles numbered 7, 8, and 9 have been shut, you may choose to roll either one or two dice.


STB::STB() : canRoll1(false) {
    for (int i = 0; i < NUM_TILES; ++i)  // initialize tiles to be open
        tiles[i] = true;
    while (play());
}



int STB::roll1() {
    srand((unsigned)time(NULL));
    return random(1, 6);
}

// roll 2 dice, returns a number between 2 and 12
int STB::roll2() {
    return roll1() + roll1();
}

// if tiles 7, 8, and 9 are shut, set canRoll1 to true and return true
bool STB::setRollOne() {
    if (not tiles[7] && not tiles[8] && not tiles[9])
        canRoll1 = true;
    return true;
}


// fixme: how to decide if the game is won or not?
bool STB::lost(int sum) {}

bool STB::won() {
    for (int i = 1; i < NUM_TILES; ++i) {
        if (tiles[i])
            return false;
    }
    return true;
}


// roll the dice, display and return result
int STB::roll() {
    int result;
    if (not canRoll1)
        setRollOne();
    if (canRoll1) {
        bool userroll1;
        cout << "Do you want to roll one dice for this round?" << endl
             << "Enter 1 if you do, else enter 0: ";
        cin >> userroll1; // fixme

        if (userroll1) {
            result = roll1();
            cout << "\nThe number you rolled is: " << result << endl;
            return result;
        }
    }
    result = roll2();
    cout << "\nThe number you rolled is: " << result << endl;
    return result;
}

// fixme: how to get an input of some unknown number of integers?

// this will be much, much easier to implement using python
// get user input and
void STB::shut() {
    // maximum number of boxes you can shut at once is 4. So expect at most 7 chars from user
    char input[7] = {'0', '0', '0', '0', '0', '0', '0'};
    cout << "Enter the tiles you want to shut, with one space between numbers" << endl;
    cin.getline(input, 7);

}

void STB::intro() {
    cout << "Welcome to Shut The Boxes! " << endl;
}


// used the strategy of first laying out the structure of play(), and then implement
// the sub-methods called by play(); inspired by 982
// returns true if game is not over, false otherwise
bool STB::play() {

    // first display the boxes still open
    display();

    // then get the number rolled
    int sum = roll();

    // check if the user loses; if so, return false so game is over
    if (lost(sum)) {
        gameover();
        return false;
    }

    // if didn't lose, ask the player to shut boxes
    shut();

    // game is won, return false to exit while loop in the ctor
    if (won()) {
        gamewon();
        return false;
    }
    // game is not over, return true so that while loop continues
    return true;

}

void STB::display() {
    cout << "Current tiles: " << endl;
    for (int i = 1; i < NUM_TILES; ++i) {
        if (tiles[i])
            cout << i << " ";
        else
            cout << "_ ";
    }
    cout << endl;

}