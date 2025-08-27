#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include "header.h"

#define X 0
#define Y 1
#define RIGHT 1
#define LEFT -1
#define DOWN 1

//cursor edges | borders?
#define FIRST_CELL_INDEX 0
#define LAST_CELL_INDEX 4


//MARK: GAME OBJECTS_START


typedef struct {
    //put ur settings here 
    int UI_width;
    int UI_height;
} Settings;


//MARK: GAME_OBJECTS_END







//MARK: Global ?
//I think settings should be global but tell me if you 
//want them to be passed as arguments






//MARK: PROTOTYPES
void run_session();
void settings_menu();
void reset_game_session(Game_Session *game_session);
void exit_prompt();
int main_menu(int window_size[2]);
void cell_grid_animation(int top, int left, int bottom, int right, int window_size[2]);
bool is_valid_word();
//idk datatype that ncurses takes as keystrokes so i just put this temprorarily
bool isLetter(int key_stroke);







//MARK: START
int main(int argc, char** argv) {
    // set up ncurses
    initscr(); // init ncurses mode
    noecho(); // suppresses character printing
    curs_set(0); // to hide the terminal cursor
    keypad(stdscr, TRUE); // to allow arrrow keys and F1-F12 keys
    start_color(); // ncurses function to start the color mode

    srand(time(NULL) * getpid());

    //verify window size block if not big enough
    int window_size[2];
    getmaxyx(stdscr, window_size[ROW], window_size[COL]);
    if (window_size[ROW] < MENU_HEIGHT || window_size[COL] < MENU_WIDTH/* to update later */) {
        curs_set(1);
        endwin();
        printf("+======================================+\n");
        printf("|  You need a bigger window to play!!  |\n");
        printf("|  Needed Rows: %3d, Your Rows: %3d %s |\n", MENU_HEIGHT, window_size[ROW], ((window_size[ROW] < MENU_HEIGHT) ? "<-" : "  "));
        printf("|  Needed Cols: %3d, Your Cols: %3d %s |\n", MENU_WIDTH, window_size[COL], ((window_size[COL] < MENU_WIDTH) ? "<-" : "  "));
        printf("|Try maximizing your window or zoom out|\n");
        printf("+======================================+\n");
        return -1;
    }
    // load last session, idk (or maybe only load it only if continue ? ) 
    
    
    int menu_input = main_menu(window_size);
    int menu_start_col = (window_size[COL] - MENU_WIDTH)/2, menu_start_row = (window_size[ROW] - MENU_HEIGHT)/2; // they start with borders included btw
    Game_Session game_session;
    // MAIN_MENU:
    do {
        switch(menu_input) {
            case NEW_GAME:
                cell_grid_animation(menu_start_row + 1, menu_start_col + 2, menu_start_row + MENU_HEIGHT-1, menu_start_col + MENU_WIDTH - 3, window_size);
                reset_game_session(&game_session); //which will pick new word
                // start timer <<<<<<<

                getch();
                // run_session();
                break;

            case CONTINUE:
                cell_grid_animation(menu_start_row + 1, menu_start_col + 2, menu_start_row + MENU_HEIGHT-1, menu_start_col + MENU_WIDTH - 3, window_size);
                getch();
                //continue session without change
                // load_previous_session();
                // run_session();
                break;

            case SETTINGS:
                getch();
                // settings_menu();//to be implemented later, any applied font setting will reload the entire UI
                break;

            case QUIT:
                goto GAME_END;
                break;

            //no need for default since do nothing! , oabd: we may need default if we have no do-while loop
        }
    } while (0 /* temporary */);
    GAME_END:
    
    
    // exit_prompt();
    
    // getch(); // temporary
    curs_set(1); // to set back the terminal cursor
    endwin(); // to end the ncurses mode
}

//MARK: END


#if 0



//MARK: DEFINITIONS

void settings_menu() { // for later

}

void exit_prompt() {

}

//MARK:gameplay

// #if 0
void reset_game_session() {
    
}


bool isLetter(int key_stroke) {
    // .......a-z....A-Z..... I basically check if it's in the dots, if yes, then it's not letter
    if(key_stroke < 'a' || key_stroke > 'Z' || (key_stroke > 'z' && key_stroke < 'A')) {
        return false;
    }
    return true;
}

/*
Write behavior:
if isEmpty():
  write letter;
  isNotLastCell(): 
       move cursor forward;
 */

 #endif