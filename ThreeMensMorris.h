//
// Created by 焦玉华 on 2018/6/2.
//

#ifndef MAXIM_THREEMENSMORRIS_H
#define MAXIM_THREEMENSMORRIS_H

struct Point {
    int a;
    int b;
};


const int WIDTH = 3;

class ThreeMensMorris {
private:
    int states[WIDTH][WIDTH];
    bool curPlayer;

    char ch(int row, int col);
    bool valid1(int row, int col);
    bool valid2(int fr, int fc, int tr, int tc);
    void displayInfo();    // used after 6 pieces are placed
    bool won();
    void play1();  // before 6 pieces are placed
    bool play2();  // after 6 pieces are placed
    void play3(int row, int col);
    void play4();

    int step1();


public:
    ThreeMensMorris();
    void display();
    void play();
    void autoplay();
};


#endif //MAXIM_THREEMENSMORRIS_H
