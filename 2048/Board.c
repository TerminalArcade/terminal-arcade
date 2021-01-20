// the implementation of the header board.h

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Board.h"

// return the pointer to a new game board
GameBoard *newGameBoard() {
    GameBoard *new_board = malloc(sizeof(GameBoard));
    
    // initialization
    for (int i = 0; i < CELL_NUM; i++) {
        new_board->cell[i] = 0;
        new_board->isNew[i] = false;
        new_board->isMerged[i] = false;
    }
    new_board->digits = 3;
    new_board->score = new_board->moveCount = 0;
    new_board->goal = 2048;

		// seed rand() for game randomness
		srand(time(NULL));

    // randomly pick 2 cells to generate initial numbers
    initCell(new_board);
    initCell(new_board);
    updateLargestCell(new_board);

    return new_board;
} 

// return the index of a cell adjacent to the given index
int cellIndex(int index, char modifier) {
    if (index < 0 || index > 15) {
        return -1;
    }
    int targetIndex;
    switch (modifier) {
        case 'u': // move up
            targetIndex = index - 4;
            break;
        case 'd': // move down
            targetIndex = index + 4;
            break;
        case 'l': // move left
            targetIndex = index - 1;
            break;
        case 'r': // move right
            targetIndex = index + 1;
            break;
        default:
            return -1;
    }
    // return the target index only when it's a valid index
    return (targetIndex >= 0 && targetIndex < CELL_NUM) ?
            targetIndex : -1;
}

// return the index of an edge in the board, based on the index and direction
int edgeIndex(int index, char direction) {
    if (index < 0 || index > 15) {
        fprintf(stderr, "non-valid index passed to edgeIndex\n");
        return -1;
    }

    int edgeIndex = index;
    switch (direction) {
        case 'u':
            edgeIndex %= 4;
            break;
        case 'd':
            while (edgeIndex < 12)
                edgeIndex += 4;
            break;
        case 'l':
            while (edgeIndex % 4 != 0)
                edgeIndex -= 1;
            break;
        case 'r':
            while (edgeIndex % 4 != 3)
                edgeIndex += 1;
            break;
        default:
            return -1;
    }
    return edgeIndex;
}

// move a single cell all the way to the end in a certain direction
void moveCell(GameBoard *board, int index, char direction) {
    int targetIndex, edge = edgeIndex(index, direction);
    bool merged = false;
    while (index != edge) {
        targetIndex = cellIndex(index, direction);
        // unmovable because on edge
        if (targetIndex == -1) break;

        if (!board->cell[targetIndex]) { // when the target is empty
            board->cell[targetIndex] = board->cell[index];
            board->cell[index] = 0;
            board->isNew[targetIndex] = board->isNew[index];
            board->isNew[index] = false;

            // no need to test if cell[index] is new, because we have merged
        } else if (board->cell[targetIndex] == board->cell[index] && !merged
                    && !board->isMerged[targetIndex]){
            // merge the cells
            board->cell[targetIndex] *= 2;
            board->cell[index] = 0;
            merged = true;
            board->isMerged[targetIndex] = true;
            board->score += board->cell[targetIndex];
        } else {
            // stop moving in that direction
            break;
        }

        // update the index
        index = targetIndex;
        if (board->cell[index] > board->largestCell) {
            board->largestCell = board->cell[index];
            // update digit count and goal value
            updateDigits(board);                
            if (board->cell[index] >= board->goal) {
                board->goal *= 2;
            }
        }
    }
}

// move the entire board in one direction
void moveBoard(GameBoard *board, char c) {
    if (c != 'u' && c != 'd' && c != 'l' && c != 'r') {
        fprintf(stderr, "unknown command to move the board\n");
        return;
    }

    // reset isNew and isMerged
    for (int i = 0; i < CELL_NUM; i++) {
        board->isNew[i] = false;
        board->isMerged[i] = false;
    }

    // initialization for nested for loops to move cells
    int init, out_incre, in_incre;
    switch (c) {
        case 'u':
            init = 4; // skipping the first line because it isn't movable.
            out_incre = 4;
            in_incre = 1;
            break;
        case 'd':
            init = 8;
            out_incre = -4;
            in_incre = 1;
            break;
        case 'l':
            init = 1;
            out_incre = 1;
            in_incre = 4;
            break;
        case 'r':
            init = 2;
            out_incre = -1;
            in_incre = 4;
            break;
    }


    // the 2 counters make sure that only 3 lines are moved and that on each
    // line, only 4 cells are moved
    for (int i = init, count = 0; count < 3; i += out_incre, count++) {
        for (int j = i, count_in = 0; count_in < 4; count_in++, j += in_incre) {
            if (board->cell[j] != 0) {
                moveCell(board, j, c);
            }
        }
    }
    // update moveCount
    board->moveCount++;
}

// print out a positive number with with a minimum length
static void printNumber(unsigned int n, int minLen, bool isNew) {
    unsigned int n_cpy = n, digits = 0;
    do {
        digits++;
    } while ((n_cpy = n_cpy/10) != 0);

    // print the preceding spaces
    for (int i = 0, range = (minLen - digits)/2; i < range; i++) {
        putchar(' ');
    }

    /* the following code prints the all extra spaces in the front
    if (digits > minLen) {
        minLen = digits;
    }
    
    for (; digits < minLen; digits++) {
        putchar(' ');
    }
    */

    // only print the number if the number is not 0
    if (n > 0) {
        if (isNew) {
            printf(RED);
        } else if (n >= 16 && n <= 64) {
            printf(YELLOW);
        } else if (n >= 128 && n <= 1024) {
            printf(MAGENTA);
        } else if (n == 2048) {
            printf(CYAN);
        } else if (n == 4096) {
            printf(GREEN);
        } else if (n >= 8192) {
            printf(BLUE);
        }
            
        printf("%i" COLOR_RESET, n);
    } else {
        // print a space in the place of each digit
        for (unsigned int i = 0; i < digits; i++) {
            putchar(' ');
        }
    }

    // print the trailing spaces
    for (int i = 0, range = (minLen-digits) - (minLen-digits)/2; i<range; i++) {
        putchar(' ');
    }
    
}

// print the board in a appropriate way
void printBoard(GameBoard *board) {
    int index = 0;

    // print general information
    printGeneralInfo(board);

    for (int i = 0; i < 5; i++) {
        putchar(' '); // start a dash line
        // print the dash line
        for (int j = 0; j < (board->digits+1) * 4; j++) {
            putchar('-');
        }
        putchar('\n');

        // print the number line
        if (i < 4) { // skip the 5 iteration where the closing line is
            putchar('|'); // start the number line
            for (int j = 0; j < 4; j++) {
                printNumber(board->cell[index], board->digits,
                            board->isNew[index]);
                index++;
                putchar('|');
            }
        }
        // newline
        putchar('\n');
    }
}

// print general information specific to a game
void printGeneralInfo(GameBoard *board) {
    printf("Get to block %i!\n", board->goal);
    printf("Score: %u\t\tSteps taken: %i\n", board->score, board->moveCount);
}

// update a board's largest digit cound based on the largest cell
void updateDigits(GameBoard *board) {
    int n = board->largestCell, newDigits = 0;
    do {
        newDigits++;
    } while ((n = n/10) > 0);
    
    if (newDigits > board->digits) {
        board->digits = newDigits;
    }
}

// update the number for largest cell based on cell[]
void updateLargestCell(GameBoard *board) {
    int max = 0;
    for (int i = 0; i < CELL_NUM; i++) {
        if (board->cell[i] > max) {
            max = board->cell[i];
        }
    }
    board->largestCell = max;
}

// return true if no possible move can be made
bool noPossibleMove(GameBoard *board) {
    // optimization: in most cases, there will an empty cell
    for (int i = 0; i < CELL_NUM; i++) {
        if (board->cell[i] == 0) {
            return false;
        }
    }

    // only 2 direction checks are necessary
    return !dirPossible(board, 'u') && !dirPossible(board, 'l');
}

bool dirPossible(GameBoard *board, char c) {
    int testIndex;
		//int edge;
    for (int i = 0; i < CELL_NUM; i++) {
        //edge = edgeIndex(i, c);
        if (board->cell[i] != 0 && (testIndex = cellIndex(i, c)) != -1 &&
            i != edgeIndex(i, c) && (board->cell[testIndex] == 0 || 
            board->cell[testIndex] == board->cell[i])) {
            return true;
        }
    }

    // possible move not found
    return false;
}

// randomly choose and initialize a cell with either 2 or 4
void initCell(GameBoard *board) {
    // manually improves the chances for getting a 2
    int n = ((rand() % 4) == 3) ? 4 : 2;
    int index;
    do {
        index = (rand() % 16);
    } while (board->cell[index] != 0);
    board->cell[index] = n;
    board->isNew[index] = true;
}

// house keeping
void endGame(GameBoard *board) {
    free(board);
}
