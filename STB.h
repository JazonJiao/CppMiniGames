// implementation of the game Shut The Box
// by Jazon Jiao, 2018.09.30
// https://brilliant.org/practice/shut-the-box-endgame-strategy/?p=1
//

#ifndef GAMES_STB_H
#define GAMES_STB_H

#include <iostream>
#include <iterator>

const int NUM_TILES = 10; // use one-based indexing; actually only has 9 boxes

struct Array;

class STB {
public:
    STB();

private:
    bool tiles[NUM_TILES]; // false if shut, true if open
    int remaining;    // number of remaining tiles
    // (it's so hard to decide if shut is represented by true or false, but true for open is better
    // since a lot of operations are based on open boxes)
    bool canRoll1;

    int roll1();
    int roll2();
    bool setRollOne();
    bool lost(int);
    bool won();

    int roll();
    void shut(int);

    void intro();
    bool play();
    void display();
    void gameover();
    void gamewon();
};

// free function
int canSum(Array array, int sum);

// a struct that is used in determining whether game is lost
// I ended up with essentially a class...
struct Array {
    //friend class STB;  // no need for friends

    int size;
    int* arr;

    explicit Array(int size) : size(size), arr(new int[size]) {}

    Array(const Array &rhs) : size(rhs.size), arr(new int[size]) {
        for (int i = 0; i < size; ++i)
            arr[i] = rhs[i];
    }

    const Array& operator=(const Array & rhs) {
        if (this != &rhs) {
            Array copy(rhs);
            std::swap(size, copy.size);
            std::swap(arr, copy.arr);
        }
        return *this;
    }

    ~Array() { delete [] arr; }

    // must return reference, else cannot be used as lvalue
    int& operator[](int i) { return arr[i]; }
    const int& operator[](int i) const { return arr[i]; }

    int sum() {
        int s = 0;
        for (int i = 0; i < size; ++i)
            s += arr[i];
        return s;
    }

    // removes the ith element of a given array
    /// Error log: not defining the copy ctor and assignment operator for Array caused segfault!!!
    /// That's when I begin to appreciate the importance of Big 3...
    // fixme: what's the difference of declaring this as static?
    // note: cannot return Array& since "array escapes local scope"
    static Array remove(const Array& rhs, int index) { // assume i is in range and rhs.size > 0
        Array tmp(rhs.size - 1);
        for (int i = 0; i < index; ++i)
            tmp[i] = rhs[i];
        for (int i = index; i < tmp.size; ++i)
            tmp[i] = rhs[i + 1];
        return tmp;
    }

    /// Error log: defined this free function in the .h file, resulting in linking error--
    /// multiple definitions since STB.cpp include .h and main.cpp includes it again
    friend std::ostream& operator<<(std::ostream& os, const Array& array);
};




#endif //GAMES_STB_H
