#ifndef HEADER_H
#define HEADER_H

#define MENU_WIDTH  102 // borders included
#define MENU_HEIGHT  55 // borders included

#define CELL_WIDTH   18 // borders excluded
#define CELL_HEIGHT   8 // borders excluded

#define WORD_LENGTH 5
#define NO_ATTEMPTS 6

# include <stdbool.h>

typedef struct {
    char history_matrix[NO_ATTEMPTS][WORD_LENGTH + 1/* for \0 to use with strcmp*/];
    char matrix_colors[NO_ATTEMPTS][WORD_LENGTH]; // for storing cell colors
    char wordle_answer[WORD_LENGTH + 1]; // to hold the word to be guessed by the player
    int current_attempt; // range(0,5) 6 attempts, helps determine what row we're at
    int cursor; // to point to a cell in a given attempt, range(0,4)
    int time_elapsed; // to store how much time the player took so far
    bool game_ended;
} Game_Session;

typedef enum {
    ROW,
    COL
} Coordinates;

typedef enum {
    NEW_GAME,
    CONTINUE,
    SETTINGS,
    QUIT
} menu_options;

#endif