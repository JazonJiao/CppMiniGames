#include <iostream>
#include <random>
#include "Maxim.h"
#include "Gomoku.h"
#include "ThreeMensMorris.h"
#include "Minesweeper.h"
#include "STB.h"
#include "Sudoku.h"
#include "G2048.h"
#include "Chomp.h"

void test() {
    int i = 2, j = 3;
    i += j *= 2;
    std::cout << i << " " << j;
}

int main() {
//    std::ofstream outFile("out.txt");

    //test();
    std::cout << "Enter the game you want to play: \n"
                 "1: Maxim\n"
                 "2: Gomoku\n"
                 "3: Three Men's Morris\n"
                 "4: Minesweeper\n"
                 "5: Shut The Box\n"
                 "6: Sudoku\n"
                 "7: 2048\n"
                 "8: Chomp\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1: {
            Maxim maxim;
            //break;
        } case 2: {
            Gomoku gomoku;
            break;
        } case 3: {
            ThreeMensMorris tmm;
            break;
        } case 4: {
            Minesweeper minesweeper;
            break;
        } case 5: {
            STB stb;
            break;
        } case 6: {
            Sudoku sudoku;
            break;
        } case 7: {
            G2048 g2048;
            break;
        } case 8: {
            Chomp chomp;
            break;
        }
        //default:
        //    std::cout << "Invalid input. Program exits.\n";
    }
    return 0;
}



