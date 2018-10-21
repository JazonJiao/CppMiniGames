/**** SUDOKU
 * This is the first game implemented with exhaustive Javadoc documentation.
 * Functionality:
 * 1. User can fill in the puzzle by entering coordinates and number through console;
 * 2. User can let the AI solve the puzzle.
 */

#ifndef GAMES_ORIG_SUDOKU_H
#define GAMES_ORIG_SUDOKU_H

#include <string>
#include <iostream>

const int SUDOKULEN = 9;

class Sudoku {
public:
    /*** 2018-10-19
     * Constructor
     * Asks the user if they want to solve the puzzle on their own.
     * If so, run the program until the puzzle is solved by user.
     * Otherwise, solve with AI and display the result.
     */
    Sudoku();

private: // private variables
    /***
     * "GIVEN" stands for grids with given values.
     * "EMPTY" stands for values that user has yet to figure out.
     * "FILLED" stands for grids that user already filled in.
     */
    enum state {GIVEN, EMPTY, FILLED};

    /***
     * It seems to be the issue with project directory, but if I only keep the
     * file name, it cannot be loaded.
     */
    const std::string FILENAME = "../Sudoku/Sudoku3.txt";

    /***
     * A 9x9 grid, with value 1~9, or 0 if it's empty.
     * Might have used 81 structs that stores row, col, and the associated number
     * in place of an static array, but that might be harder to implement.
     */
    int grid[SUDOKULEN][SUDOKULEN];

    /***
     * A 9x9 grid of states.
     * In this implementation, if grid[i][j] is 0, then states[i][j] must be EMPTY.
     */
    state states[SUDOKULEN][SUDOKULEN];



private:  // private methods
    /*** 2018-10-17
     * Reads the 9x9 grid from a file.
     * Used for initialization of grid and states in the ctor.
     * The file contains 81 numbers on 9 lines, 0 for empty grid, 1~9 for given numbers.
     * The file is assumed to contain a valid Sudoku puzzle; there's no range-checking.
     * File's name is set as a private const variable.
     */
    void readGrid();

    /*** 2018-10-17
     * Displays the 9x9 grid. Much code is borrowed from Minesweeper::display().
     * When displaying, indices of cols and rows are from 1 to 9,
     * although the 2D array is 0-based index.
     * The interface is not pretty, but it's the best I can do in a console game.
     */
    void display() const;

    /*** 2018-10-17
     *
     * @return true if the coordinate does not contain a GIVEN value,
     *         and if the number entered satisfies the rules of Sudoku,
     *         i.e. each digit (as of now) only appears once in each row / column / 3x3 grid.
     *         For simplicity of implementation, if the grid is FILLED,
     *         then return true only if the user enters a different number than what's filled.
     * Alternatively, user can put a 0 on a grid that's been previously filled,
     * effectively resetting it to EMPTY state.
     * Might have implemented this to return an int that indicates the reason for invalid entry.
     */
    bool isValid(int row, int col, int n) const;

    /*** 2018-10-18
     * Mark the grid to the corresponding number, or reset it to EMPTY.
     * The input is assumed to be valid; range-checking is performed in isValid().
     */
    void mark(int row, int col, int n);

    /*** 2018-10-17
     * Play one round. Player enters three numbers: row, column, and number to put in.
     *
     * Note: for simplicity, unlike previous programs, where #col is entered before #row,
     * here #row is entered before #col.
     * This is unlike Cartesian coordinates, and more like Matrix index notation.
     *
     * Then decrement the row & col entered so they match the 0-based indexing of the arrays.
     * Then check if the input is valid;
     * if so, set / overwrite the corresponding grid to the number, otherwise return false.
     * Finally, check if the puzzle is solved; if so return false so that game ends.
     * Otherwise true is returned, and the while loop in the ctor continues, i.e. game continues.
     *
     * @return true if puzzle is not yet solved, or if the user's input is invalid
     */
    bool play();

    /*** 2018-10-18
     *
     * @return true if the puzzle is solved, i.e. no block is EMPTY.
     * If any entry is invalid, should be taken care by isValid()
     * Scans the grid from bottom right to top left to save time for AI solver,
     * which fills the grid from top left to bottom right.
     */
    bool solved() const;

    /**********************
     * Now for the AI part...
     */

    /*** 2018-10-19
     * Solves the Sudoku puzzle using simple backtracking.
     * Should always return true if the puzzle is a valid Sudoku puzzle.
     * Solves from left to right, and from the top down.
     * coord is row * 9 + col, so when calling solver(coord + 1),
     * it solves from the next column in the same row, or the first column in the next row.
     */
    bool solver(int coord);

    /*** 2018-10-19
     * Set the corresponding grid to the number; or remove it, resetting it to 0.
     * Update the state of that grid to be FILLED / EMPTY as well.
     * No validity-checking performed since it's used by the AI solver.
     */
    void set(int row, int col, int n);
    void remove(int row, int col);
};


#endif //GAMES_ORIG_SUDOKU_H
