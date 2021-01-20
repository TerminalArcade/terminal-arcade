// the driver program for a 2048 Command Line game

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "Board.h"
#include "Save.h"

void printIntro();
char getFirstChar();
char getLastChar();

int main() {
    printIntro();
    puts(CYAN "<<< PRESS ENTER TO START THE GAME >>>" COLOR_RESET);
    getLastChar();

    SaveData sd;

    // read previous file
    FILE *prevSave = fopen(".cl2048_save", "r");
    if (!prevSave) {
        printf("Save file not found. Create a new save file? (y/n) ");
        char response;

        // keeps promting if input is not y or n
        while ((response = getFirstChar()) != 'y' && response != 'n') {
            printf("yes or no? ");
        }

        // exit if user typed n
        if (response == 'n') {
            printf("The game needs a save file to run.\n");
            printf("Exitting game...\n");
            return 0;
        }
        // keeps going if user typed y
        memset(&sd, 0, sizeof(SaveData)); // initialize save data to 0
    } else {
        // get the old save information
        fread(&sd, sizeof(SaveData), 1, prevSave);
        fclose(prevSave);
    }

    // start the game
    GameBoard *board = newGameBoard();
    printf("\t\t\t\tHistory high: %u\n", sd.highestScore);
    printBoard(board);

    int c;
    // controls
    int up = 'w', down = 's', left = 'a', right = 'd';

    do {
        // quite the game if input starts with q
        if ((c = tolower(getLastChar())) == 'q') {
            break;
        }
        if (c == down) {
            c = 'd';
        } else if (c == left) {
            c = 'l';
        } else if (c == right) {
            c = 'r';
        } else if (c == up) {
            c = 'u';
        }
        
        if (c!='u' && c!='d' && c!='l' && c!='r') {
            printf("%c is not a valid move\n", c);
        } else if (!dirPossible(board, c)) {
            printf("You cannot move that way!\n");
        } else {
            moveBoard(board, c);
            initCell(board);
        }
        
        if (sd.highestScore > board->score) {
            printf("\t\t\t\tHistory high: %u\n", (sd.highestScore > board->score) ?
                                                  sd.highestScore : board->score);
        }
        printBoard(board);

    } while (!noPossibleMove(board));

    // before terminating program

    // update save file
    FILE *sf = fopen(".cl2048_save", "w");
    if (!sf) {
        fprintf(stderr, "Default save filename inaccessible. Please specify\n");
        fprintf(stderr, "your own save filename: (100 characters max)\n");
        char filename[100];
        filename[99] = '\0';
        scanf("%99s", filename);
        sf = fopen(filename, "w");
    }

    // save the buffer savefile into the actual save file
    if (sd.highestScore < board->score) {
        sd.highestScore = board->score;
    }

    fwrite(&sd, sizeof(SaveData), 1, sf);
    // remove(".cl2048_save_buf");
    fclose(sf);

    endGame(board);
    return 0;
}

void printIntro() {
    puts("Welcom to 2048 Command Line!\n");
    puts("This is a where you can play the beloved game 2048 in a\n");
    puts("command-line interface. In the game, you need to move\n");
    puts("and merge cells that have the same number to get higher\n");
    puts("score and larger cells. The bigger cell you reach, the\n");
    puts("higher your score is going to get! Have fun!\n");
    putchar('\n');
    puts("Controls are mapped to wasd; enter q to quit the game");
    putchar('\n');
    puts("ps: Strategy MATTERS!!!\n");
    putchar('\n');
}

char getLastChar() {
    int c = getchar(), temp;
        // doesn't execute if the user simply hits enter
    while (c != '\n' && (temp = getchar()) != EOF && temp != '\n') {
        c = temp;
    }
    return c;
}

// get the char of user input
char getFirstChar() {
    int c = getchar(), temp;
        // doesn't execute if the user simply hits enter
    while (c != '\n' && (temp = getchar()) != EOF && temp != '\n');
    return c;
}
