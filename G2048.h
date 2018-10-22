/**** 2048
 * Functionality:
 * 1. User can play the game 2048 even after 2048 is reached, until s/he loses.
 * 2. (New!) User can reverse steps as far back as s/he wants.
 * 3. todo: AI SOLVER
 */

#ifndef GAMES_ORIG_G2048_H
#define GAMES_ORIG_G2048_H

const int LEN_2048 = 4;

/***
 * Used for storing the history of grids, so user can reverse steps
 */
struct Node2048 {
    int grids[LEN_2048][LEN_2048];
    int nEmpty;  /// Error log: forgetting this caused errors
    Node2048* next;
};

class G2048 {
public:
    G2048();

    /*** 2018-10-21
     * Don't forget to free the linked list
     */
    ~G2048();

private:
    /***
     * A 4x4 grid that holds the numbers.
     * Originally implemented with enum num {EMPTY = 0, P1 = 2, P2 = 4, ... P10 = 1024},
     * with Pi representing the ith power of 2.
     * However, this makes it impossible to update each grid through multiplication by 2.
     */
    int grid[LEN_2048][LEN_2048];

    /***
     * Number of empty grids remaining. This parameter is always consistent with
     * the data in grid[][], and is used for generating new numbers.
     */
    int numEmpty;

    /***
     * addedScore is set for each round in move() and reset each round in play().
     * Score is added by addedScore for each round in play().
     */
    int addedScore;
    int score;

    /***
     * Random probability of generating a 4 instead of 2
     * if it is 0, then inside generate(), the part of determining whether the
     * new grid will be 2 or 4 is skipped
     */
    const double PROB_4 = 0.05;

    /***
     * A stack-like linked list, with head pointing to the latest grid.
     * The tail of the list is the grid before the first round, containing only 2 numbers.
     */
    Node2048* head;

    /***
     * Useless variable to keep track of number of rounds played (and # of nodes in linked list + 1)
     */
    int rounds;

    /***
     * This variable is set to true at the start, and set to false when 2048 is reached.
     * That will prevent the program from keeping to call gameWon() after 2048 is reached.
     */
    bool notWon;

private:

    /***2018-10-18,19
     * Displays the 4x4 grid, the total score, and the score gained in this round
     */
    void display() const;
    void displayLine(int line) const;

    /*** 2018-10-19,20
     * Prompts for user's move (up/left/down/right), case insensitive;
     * if input is invalid, return true immediately so that program can prompt again.
     * (CAVEAT: if user enters multiple valid characters, program will execute consecutively.)
     *
     * If user wants to revert one round, invoke reverse(), display and return true.
     * Or if the input is a valid move, update the score, the grids, and # of rounds;
     * generate a new number on a random empty grid; and record the current grid.
     *
     * Then check if the game is lost / won;
     * if so, display endgame information before returning false to the ctor.
     * Otherwise, return true to the ctor so that game continues.
     */
    bool play();

    /*** 2018-10-18,19
     * Generates a 2 or a 4 on a random empty grid, and sets that grid to 2 or 4.
     * The random generator used is copied from the professor's sample code and the internet,
     * and is different from the method used before.
     * After the grid is set, numEmpty is decremented.
     */
    void generate();

    /*** 2018-10-19
     * Moves the grids to fill empty spots, then merge if there are adjacent identical blocks,
     * and finally moves again to fill empty spots.
     * addedScore is updated by the sum of numbers merged.
     * Returns true if any block is moved or merged, false otherwise.
     *
     * Could have used addedScore == 0 as a condition for any grid changing,
     * but didn't do so since I introduced the addedScore variable only later.
     *
     * Added functionality: can reverse one round if user enters 'r'.
     */
    bool move(char choice);

    /*** 2018-10-19
     * The four move methods are similarly implemented.
     * They return true if any block is moved, false otherwise.
     */
    bool moveU();
    bool moveD();
    bool moveL();
    bool moveR();

    /*** 2018-10-19
     * Merges adjacent identical grids, and update addedScore and numEmpty.
     * They return true if any block is merged, false otherwise.
     * At first I thought mergeU and mergeD are the same, so is mergeL and mergeR,
     * but later found one (annoying) case where they should behave differently.
     * For example, for 2 2 2, mergeL/U should give 4 2, while mergeR/D should give 2 4.
     */
    bool mergeU();
    bool mergeD();
    bool mergeL();
    bool mergeR();

    /*** 2018-10-19,21
     * lost() returns true if no grid is empty and no adjacent grid holds the same number.
     * won() returns true if there is a 2048 grid anywhere; if so, set notWon to false.
     */
    bool lost() const;
    bool won();

    /*** 2018-10-21
     * After lost() returned true, ask the user whether to reverse one round
     * Return true if user does (then the previous step is recovered and game continues),
     * false otherwise (then program ends).
     */
    bool gameLost();

    /*** 2018-10-21
     * 2048 is reached.
     * Return true if user wants to keep playing, false otherwise.
     */
    bool gameWon() const;

    /*** 2018-10-20
     * A function that enables reversing steps.
     * Push onto the stack of Nodes the current grid.
     */
    void logProgress();

    /*** 2018-10-20
     * Restore the grid and numEmpty in the previous step.
     * Pop the head of linked list of past grids, and delete the head.
     * Return false if already reached the first round,
     * i.e., head contains the grid after the initial grid is generated.
     */
    bool reverse();

    /***
     * I can actually get a decent score just by entering ssadssadssad...
     */
    //bool autoplay();
};



#endif //GAMES_ORIG_G2048_H
