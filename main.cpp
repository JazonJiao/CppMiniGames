#include <iostream>
#include "Maxim.h"
#include "Gomoku.h"
#include "ThreeMensMorris.h"
#include "Minesweeper.h"
//#include "STB.h"



int main() {
//    std::ofstream outFile("out.txt");

    std::cout << "ENter the game you want to play: \n"
                 "1: Maxim\n"
                 "2: Gomoku\n"
                 "3: Three Men's Morris\n"
                 "4: Minesweeper\n"
                 "5: Shut The Box\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1: {
            Maxim maxim;
            break;
        } case 2: {
            Gomoku gomoku;
            break;
        } case 3: {
            ThreeMensMorris threeMensMorris;
            break;
        } case 4: {
            Minesweeper minesweeper;
            break;
        } case 5: {
            //STB stb;
            break;
        }
        //default:
        //    std::cout << "Invalid input. Program exits.\n";
    }
    return 0;
}



