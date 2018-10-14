// implementation of the game Shut The Box
// by Jazon Jiao, 9/30/2018 and 10/13/2018
// https://brilliant.org/practice/shut-the-box-endgame-strategy/?p=1
//

#include "STB.h"
#include <random>
#include <ctime>
#include <assert.h>

#define random(a, b) (rand() % (b-a+1) + a)

using namespace std;

// GAMEPLAY
// Each turn, you the player rolls two dice, and add their sum. Then you shut one or more boxes
// as long as the sum of the number on these boxes equal the sum of two dice.
// For example, if you roll an 8, the following combinations of tiles could be shut:
// (8), (7,1), (6,2), (5,3), (4,3,1), (5,2,1).
// If there's no way you can shut any boxes, then you lose. When all boxes are shut, you win.
// If the tiles numbered 7, 8, and 9 have been shut, you may choose to roll either one or two dice.

// whether player can roll only 1 dice, i.e. if tiles 7,8,9 are all shut
STB::STB() : remaining(9), canRoll1(false) {
    for (int i = 0; i < NUM_TILES; ++i)  // initialize tiles to be open
        tiles[i] = true;

    intro();
    while (play());
}

int STB::roll1() {
    srand((unsigned)time(nullptr));
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


// returns true if no matter how the user chooses tiles to shut,
// the tile's numbers can't add up to number rolled
bool STB::lost(int numRolled) {
    assert(remaining > 0); // this should be guaranteed
    Array array(remaining);
    /// Error log: tiles[0] is useless, i should start at 1, not 0...
    for (int i = 1, j = 0; i < NUM_TILES; ++i) { // construct an array that holds remaining tiles
        if (tiles[i]) {
            array[j] = i;
            ++j;
        }
    }
    int notLost = canSum(array, numRolled);
    //std::cout << notLost;
    return notLost == 0;
}

// free function
// dynamic programming problem: determine if there exists a subset that add up to a number
// return a positive number if a subset of array can add up to sum, 0 otherwise
int canSum(Array array, int sum) { // array is guaranteed to be non-empty
    //std::cout << array;
    if (sum < 0) {
        return 0; // subset not found
    }
    if (sum == 0) {
        return 1; // subset found
    }
    if (array.size == 1) {  // another base case
        return array[0] == sum;
    }
    int result = 0;
    for (int i = 0; i < array.size; ++i) {
        result += canSum(Array::remove(array, i), sum - array[i]);
    }
    return result;
}

// return true if all tiles are shut
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
    // first check if user can roll only one dice from this round on
    if (not canRoll1)
        setRollOne();  // if conditions are satisfied (tiles 7,8,9 closed), canRoll1 set to true
    if (canRoll1) {
        bool userroll1;
        cout << "Do you want to roll only \none dice for this round?" << endl
             << "Enter 1 if you do, else 0: ";
        cin >> userroll1; // fixme

        if (userroll1) {
            result = roll1();
            cout << "\nThe number you rolled is: " << result << endl;
            return result;
        }
    } else {
        result = roll2();
        cout << "\nThe number you rolled is: " << result << endl;
    }
    return result;
}

// question: how to get an input of some unknown number of integers?
// note: this will be much, much easier to implement using python
// get user input and shut the corresponding boxes
void STB::shut(int numRolled) {
    bool valid; // whether the user's input is valid
    int sum = 0, userInput = 0, numShut = 0;

    bool buffer[NUM_TILES];  // a temporary buffer for storing current tile conditions
    for (int i = 0; i < NUM_TILES; ++i)
        buffer[i] = tiles[i];  // fill the buffer

    do {
        valid = true;
        cout << "Enter the tiles you want to shut, \nwith a 0 at the end: " << endl;
        while (true) { // get user input until user enters 0
            cin >> userInput;
            if (userInput == 0)
                break;
            if (userInput < 0 || userInput >= NUM_TILES) {
                valid = false;  // input is not in range 0 to 9
                break;
            } else if (not buffer[userInput]) {  // the corresponding box is shut
                valid = false;
                break;
            } else {
                buffer[userInput] = false;  // shut the box
                sum += userInput;  // update sum
                ++numShut;
            }
        }
        if (not valid || sum != numRolled) { // input not in range or sum is not equal to numRolled
            valid = false;    // update valid in case sum != numRolled
            cout << "Invalid input. Try again: \n";
        } else {
            std::swap(buffer, tiles); // if everything is alright, swap the buffer
            remaining -= numShut;    // update number of tiles remaining
        }
    } while (not valid);

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
    int numRolled = roll();

    // check if the user loses; if so, return false so game is over
    if (lost(numRolled)) {
        gameover();
        return false;
    }

    // if didn't lose, ask the player to shut boxes
    shut(numRolled);

    // game is won, return false to exit while loop in the ctor
    if (won()) {
        gamewon();
        return false;
    }
    // game is not over, return true so that while loop in ctor continues
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

void STB::gameover() {
    cout << "Sorry, you lost! \n"
            "No combination of tiles add up\n"
            "to the number rolled.";
}

void STB::gamewon() {
    cout << "Congrats, you won!";
}


std::ostream& operator<<(std::ostream& os, const Array& array) {
    for (int i = 0; i < array.size; ++i) {
        os << array[i] << " ";
    }
    os << "\n";
//    std::copy(&array[0], &array[array.size],
//              std::ostream_iterator<int>(os, " "));
}

//void STB::shut() {
//    bool valid = true; // whether the user's input is valid
//    // maximum number of boxes you can shut at once is 4. So expect at most 7 chars from user
//    char input[7] = {'0', '0', '0', '0', '0', '0', '0'};
//    do {
//        cout << "Enter the tiles you want to shut, with one space between numbers" << endl;
//        cin.getline(input, 7);
//        for (int i = 0; i < 7; i += 2) {
//            if (input[i] > '9' || (input[i] != '0' && not tiles[i]))
//                // discarded since need to convert char to int
//                valid = false;
//        }
//    } while (not valid);
//
//}