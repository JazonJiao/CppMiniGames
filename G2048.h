//
// Created by 焦玉华 on 2018-10-18.
//

#ifndef GAMES_ORIG_G2048_H
#define GAMES_ORIG_G2048_H

const int LEN_2048 = 4;

class G2048 {
public:
    G2048();

private:
//    /***
//     * P1 to P10 represent the powers of 2 from 2^1 = 2 to 2^10 = 2048.
//     */
//    enum num {EMPTY = 0, P1 = 2, P2 = 4, P3 = 8, P4 = 16, P5 = 32,
//            P6 = 64, P7 = 128, P8 = 256, P9 = 512, P10 = 1024};
    int grid[LEN_2048][LEN_2048];

    /***
     * Number of empty grids remaining. This parameter is always consistent with
     * the data in grid[][], and is used for generating new numbers.
     */
    int numEmpty;

    int score;

    /***
     * Random probability of generating a 4 instead of 2, usually 0.1 to 0.25
     */
    const double PROB_4 = 0.15;


private:

    void display() const;
    void displayLine(int line) const;

    /*** 2018-10-19
     *
     * @return
     */
    bool play();

    /*** 2018-10-18
     * Generates a 2 or a 4 on a random empty grid, and sets that grid to 2 or 4.
     * The random generator used is copied from the professor's sample code and the internet,
     * and is different from the method used before.
     * After the grid is set, numEmpty is decremented.
     */
    void generate();

    void moveU();
    //void moveL();
   // void moveR();
    //void moveD();

};



#endif //GAMES_ORIG_G2048_H
