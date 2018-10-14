//
// Created by 焦玉华 on 2018-07-21.
// Total hours taken to finish program: about 6~7 hrs

#include <iostream>
#include <ctime>
#include <cassert>
#include <fstream>
#include "Minesweeper.h"

// when initialized, the center grid is exempt from any bombs,
// and will be automatically revealed
Minesweeper::Minesweeper() : numMines(0), start(0), end(0) {
    readHS();

    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) { /// Error log: i < H......
            contents[i][j] = 0;
            states[i][j] = UNTOUCHED;
        }
    }

    srand((unsigned)time(nullptr));
    double rd;
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            rd = (double) rand() / RAND_MAX;  // 0 <= rd <= 1
            // the center grid will not contain mines
            if (rd < difficulty && (i != W/2 || j != H/2)) {
                contents[i][j] = MINE;
                numMines++;
                updatecontent(i, j);
            }
        }
    }
    // reveal the center grid
    reveal(W / 2, H / 2);

    // NOTE: for this program, every line on console should <= 40 chars
    std::cout << "\n---Welcome to Mine Sweeper!--- \n"
            "For each round, please enter 3 numbers:\n"
            "#1 for column number, #2 for row number,\n"
            "and #3 = 1 if you want to reveal it, \n"
            "= 2 to flag it (you think it's a mine).\n"
            "Good luck!\n\n";
    //gameover(); // for testing purpose

    start = time(nullptr);  // get current time
    while (play());  // play the game until game over

}

// note: actually row and col should swap
void Minesweeper::updatecontent(int row, int col) {
    int curR, curC;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            curR = row + i;
            curC = col + j;
            if (valid(curR, curC) &&
                    contents[curR][curC] != MINE)
                contents[curR][curC]++;
        }
    }

}

void Minesweeper::flag(int x, int y) {
    assert(valid(x, y));
    states[x][y] = FLAGGED;
}

// assumes input location isn't a mine
void Minesweeper::reveal(int x, int y) {
    if (not valid(x, y) || states[x][y] == REVEALED) // x, y goes out of range or grid is already revealed
        return;

    states[x][y] = REVEALED;
    if (contents[x][y] == 0)  // this grid is empty, recursively reveal all 8 adjacent grids
        for (int i = -1; i < 2; i++)
            for (int j = -1; j < 2; j++)
                if (i != 0 || j != 0)  // i and j are not both 0
                    reveal(x + i, y + j);

}

bool Minesweeper::valid(int x, int y) {
    return x >= 0 && x < W && y >= 0 && y < H;// && states[x][y] != REVEALED;
}

// returns true if game continues, false if game is over
bool Minesweeper::play() {
    display();

    std::string str;
    int x, y, op;
    std::cout << "Please enter your next move: ";
    std::cin >> x >> y >> op;
    std::getline(std::cin, str); // consume current line

    // change to 1 <= op <= 3 if QUESTION is enabled
    if (not valid(x, y) || (op != 1 && op != 2)) {
        std::cout << "Invalid input.\n";
        return true;
    }

    if (op == 2) {
        flag(x, y);
    } else if (contents[x][y] == MINE) {  // op == 1
        gameover();
        return false;  // game is over
    } else {
        reveal(x, y);
    }
    if (checkwon()) {
        gamewon();
        return false;
    } else {
        return true;
    }
}

// Criteria for winning: all grids that aren't mines are revealed
// or all flags correspond to each and every mine
bool Minesweeper::checkwon() {
    int numCorrect = 0;
    int numRevealed = 0;
    for (int i = 0; i < W; i++) {
        for (int j = 0; j < H; j++) {
            if (states[i][j] == FLAGGED && contents[i][j] == MINE)
                numCorrect++;
            if (states[i][j] == REVEALED)
                numRevealed++;
        }
    }
    return numCorrect == numMines || numRevealed == (W * H - numMines);
}

// the format of MinesweeperHS.txt:
// 1(rank) default(username) 100(seconds used)
// ... (for HIGHSCORERS entries)
void Minesweeper::readHS() {
    int i = 0, score;
    std::string name;
    std::ifstream file(FILENAME.c_str());
    assert(file);
    file >> score;  // consume the first rank

    do {
        file >> name;
        names[i] = name;
        file >> score;
        scores[i] = score;
        i++;
    } while (file >> score);
}

void Minesweeper::writeHS(double newHS) {
    std::cout << "\n\nNEW HIGH SCORE!\n"
            "Please enter your name (<= 9 chars):\n";

    std::string username;
    std::cin >> username;
    if (username.length() > 9)
        username = username.substr(0, 9);  // truncate

    int i = HIGHSCORERS - 1;
    while (i > 0) { // insert the new high score into the array of HS
        // assumes the new score is already higher than the last entry
        if (scores[i - 1] > newHS) {
            scores[i] = scores[i - 1];
            names[i] = names[i - 1];  /// Error log: forgot to move names...
        }
        else break;
        i--;
    }
    // note the location of i
    scores[i] = newHS;
    names[i] = username;

    std::ofstream file(FILENAME.c_str());
    for (int r = 0; r < HIGHSCORERS; r++) {
        file << (r+1) << " " << names[r] << " " << scores[r] << std::endl;
    }
}

void Minesweeper::displayHS() {
    std::cout << "RANK - NAME - SCORE\n";
    for (int i = 0; i < HIGHSCORERS; i++) {
        printf("%2d%10s  %.0f\n", i+1, names[i].c_str(), scores[i]);
        /// Error log: failure to convert to c_str will print trash...
    }
}

void Minesweeper::gamewon() {
    end = time(nullptr);
    double diff = difftime(end, start);
    std::cout << "\nCongrats! You won the game!"
            "\nTime taken: " << diff << " s."
            "\n\n  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;
    for (int i = 0; i < W + 2; i++)
        std::cout << "+-";
    std::cout << "+" << std::endl;

    // difference from display() is how I print the middle parts
    for (int j = 0; j < H; j++) {
        std::cout << j % 10 << ">|";
        for (int i = 0; i < W; i++) {
            if (contents[i][j] == MINE)
                std::cout << "*";
            else if (contents[i][j] == 0)
                std::cout << " ";
            else
                std::cout << contents[i][j];
            std::cout << "|";
        }
        std::cout << "<" << j << std::endl;
    }

    for (int i = 0; i < W + 2; i++)
        std::cout << "+^";
    std::cout << "+" << std::endl << "  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;

    // the part dealing with high scores
    if (diff < scores[HIGHSCORERS - 1]) {  /// Error log: used > here???
        writeHS(diff);
        readHS();
        displayHS();
    }
}

void Minesweeper::gameover() {
    end = time(nullptr);
    double diff = difftime(end, start);
    std::cout << "\nSorry--You hit a bomb!! GAME OVER."
            "\nTime taken: " << diff << " s."
            "\nF is any mine you correctly identified;"
            "\n* represents mines you've not yet found;"
            "\nW is where flagged but there's no mine;"
            "\n\n  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;
    for (int i = 0; i < W + 2; i++)
        std::cout << "+-";
    std::cout << "+" << std::endl;

    // difference from display() is how I print the middle parts
    for (int j = 0; j < H; j++) {
        std::cout << j % 10 << ">|";
        for (int i = 0; i < W; i++) {
            if (states[i][j] == FLAGGED) {
                if (contents[i][j] == MINE)
                    std::cout << "F";
                else
                    std::cout << "W";
            }
            else if (contents[i][j] == MINE)
                std::cout << "*";
            else if (contents[i][j] == 0)
                std::cout << " ";
            else
                std::cout << contents[i][j];
            std::cout << "|";
        }
        std::cout << "<" << j << std::endl;
    }

    for (int i = 0; i < W + 2; i++)
        std::cout << "+^";
    std::cout << "+" << std::endl << "  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;
}

// assumes that game isn't over (no bombs are revealed)
// when row or col > 10, displays the last digit of that row/col.
void Minesweeper::display() {
    // print first two lines
    std::cout << "\n  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;
    for (int i = 0; i < W + 2; i++)
        std::cout << "+-";
    std::cout << "+" << std::endl;

    // print middle part; different from previous programs,
    // rows are printed with small-index rows at the top
    for (int j = 0; j < H; j++) {
        std::cout << j % 10 << ">|";
        for (int i = 0; i < W; i++) {
            if (states[i][j] == UNTOUCHED)
                std::cout << "-";
            else if (states[i][j] == FLAGGED)
                std::cout << "*";
//            else if (states[i][j] == QUESTION)
//                std::cout << "?";
            else if (contents[i][j] == 0)
                std::cout << " ";
            else
                std::cout << contents[i][j];
            std::cout << "|";
        }
        std::cout << "<" << j << std::endl;
    }

    // print final two rows
    for (int i = 0; i < W + 2; i++)
        std::cout << "+^";
    std::cout << "+" << std::endl << "  |";
    for (int i = 0; i < W; i++) {
        if (i < 10)
            std::cout << i << "|";
        else
            std::cout << i;
    }
    std::cout << std::endl;
}