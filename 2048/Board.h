#ifndef GAMEBOARD_2048
#define GAMEBOARD_2048

// the board header that contains functions to manipulate a 2048 game board

#include <stdbool.h>
#include <stdlib.h>

#define CELL_NUM 16
// macros for the indeics for the cells, respectively
#define UPPER_LEFT 0
#define UPPER_RIGHT 3
#define LOWER_LEFT 12
#define LOWER_RIGHT 15

// define colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

typedef struct GameBoard {
    int cell[CELL_NUM];
    int digits;
    int largestCell;
    int goal;
    unsigned int moveCount;
    unsigned int score;
    bool isNew[CELL_NUM];
    bool isMerged[CELL_NUM];
} GameBoard;

// allocate memory and initialize values for a new GameBoard
// return the pointer to that GameBoard
GameBoard *newGameBoard();

// make a move in the game board with the corresponding command
void moveBoard(GameBoard *board, char command);

// move a single cell all the way to the end in a direction
void moveCell(GameBoard *board, int index, char direction);

// print the game board
void printBoard(GameBoard *board);

// print general info specific to the game
void printGeneralInfo(GameBoard *board);

// return true if there is no move to be made, false otherwise
bool noPossibleMove(GameBoard *board);

// return true if the board can be move in the given direction
bool dirPossible(GameBoard *board, char c);

// do the housekeeping after a game
void endGame(GameBoard *board);

// randomly choose and initialize a cell to 2 or 4 in board
void initCell(GameBoard *board);

// return the index of the cell adjacent to the given cell
int cellIndex(int index, char modifier);

// return the index at the edge of the board
int edgeIndex(int index, char direction);

// update the largest digit count based on the largest cell found
void updateDigits(GameBoard *board);

void updateLargestCell(GameBoard *board);

#endif
