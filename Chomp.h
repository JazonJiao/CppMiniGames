/*** CHOMP
 * https://brilliant.org/practice/poisoned-chocolate-ii/?p=1
 * TODO: AI player
 */

#ifndef GAMES_ORIG_CHOMP_H
#define GAMES_ORIG_CHOMP_H

class Chomp {
public:
    Chomp();
    ~Chomp();

private:

    const int MAX_ROW = 16, MAX_COL = 16;

    /***
     * The height of chocolate bar, length of dynamic array
     */
    int height;

    /***
     * A dynamic array that stores the number of chocolates in each row.
     * When initialized, each entry contains the width of the rectangle.
     */
    int* arr;

    /***
     * Set to false if player 1 is playing; true if player 2 is playing.
     */
    bool curPlayer;

private:

    /*** 2018-10-21
     * A valid move is chopping any grid that contains a chocolate
     * (except the top-left poisonous one)
     */
    bool valid(int row, int col) const;

    /*** 2018-10-21
     * display the grid with the poisoned grid at the top left
     */
    void display() const;

    /*** 2018-10-21
     * Play one round. Return false if one of the player loses.
     */
    bool play();

    /*** 2018-10-21
     * Remove all grids to the lower-right of the coordinate
     */
    void chop(int row, int col);

    /*** 2018-10-21
     * Return true if only the top-left block is left
     */
    bool lost() const;

};


#endif //GAMES_ORIG_CHOMP_H
