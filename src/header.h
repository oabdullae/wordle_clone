#ifndef HEADER_H
#define HEADER_H

#define MENU_WIDTH  102 // borders included
#define MENU_HEIGHT  55 // borders included

#define CELL_WIDTH   18 // borders excluded
#define CELL_HEIGHT   8 // borders excluded

#define WORD_LENGTH 5
#define NO_ATTEMPTS 6

//cursor edges | borders?
#define FIRST_CELL_INDEX 0
#define LAST_CELL_INDEX 4

// cursor_movements
#define RIGHT 1
#define LEFT -1
#define DOWN 1


# include <stdbool.h>

typedef struct {
    char history_matrix[NO_ATTEMPTS][WORD_LENGTH + 1/* for \0 to use with strcmp*/];
    int matrix_colors[NO_ATTEMPTS][WORD_LENGTH]; // for storing cell colors
    char wordle_answer[WORD_LENGTH + 1]; // to hold the word to be guessed by the player
    int current_attempt; // range(0,5) 6 attempts, helps determine what row we're at
    int entered_letters; // holds the number of letters entered in an attempt
    int cursor; // to point to a cell in a given attempt, range(0,4)
    int time_elapsed; // to store how much time the player took so far
    bool game_ended;
    int menu_start_row, menu_start_col; 
} Game_Session;

typedef struct {
    char buffer[CELL_HEIGHT][CELL_WIDTH+1]; // +1 for \0
} Ascii_Art_Letter;

typedef enum {
    UNIVERS,
    DOOM,
    VARSITY
} Fonts;

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

typedef enum{
    NO_COLOR = 10, // undetermined 
    GREEN, // letter exists in correct position
    YELLOW, // letter exists in wrong position
    GRAY, // letter does not exist
    BLUE, // cursor color
    RED // for invalid word warning
} Cell_Colors;

typedef enum {
    DELETE_CURSOR, // to tell src/graphics.c:change_cursor() to delete the cursor
    PRINT_CURSOR // to tell it to print the cursor
} Cursor_Actions;

#endif