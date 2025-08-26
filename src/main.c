#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#define X 0
#define Y 1
#define RIGHT 1
#define LEFT -1
#define DOWN 1

//cursor edges | borders?
#define FIRST_CELL_INDEX 0
#define LAST_CELL_INDEX 4

typedef enum {
    ROW,
    COL
} Coordinates;

//temporary, just here so i don't get errors, ncurses can detect keystores anyway
typedef enum {
    LEFT_KEY,
    RIGHT_KEY,
    ENTER_KEY,
    BACK_SPACE_KEY,
} Key_stroke;
//MARK: GAME OBJECTS_START
enum Options{//turn this into define or smth later
    NEW_GAME,
    CONTINUE,
    SETTINGS,
    QUIT
};

typedef struct {
    //put ur settings here 
    int UI_width;
    int UI_height;
} Settings;

struct { // for storing the state of the game and can be used for the Continue feature
    int number_of_used_attempts;
    int cursor_position[2];
    char wordle_answer[6];//5 characters + \0

    //stores the matrix of your previous guesses in any game session
    //6 chars again because of line will have [ ][ ][ ][ ][ ][\0] (so we can use strcmp)
    char history_matrix[6][6];
    bool game_ended; // true if game is not running
} game_session; // game_session is a single variable that holds the game session
//MARK: GAME_OBJECTS_END







//MARK: Global ?
//I think settings should be global but tell me if you 
//want them to be passed as arguments






//MARK: PROTOTYPES
void run_session();
void settings_menu();
void reset_game_session();
void exit_prompt();
int main_menu(int window_size[2]);
//idk datatype that ncurses takes as keystrokes so i just put this temprorarily
bool isLetter(int key_stroke);







//MARK: START
int main(int argc, char** argv) {
    // set up ncurses
    initscr(); // init ncurses mode
    noecho(); // suppresses character printing
    curs_set(1); // to hide the terminal cursor
    start_color(); // ncurses function to start the color mode

    //verify window size block if not big enough
    int window_size[2];
    getmaxyx(stdscr, window_size[ROW], window_size[COL]);
    if (window_size[ROW] < 70 /* to update later */|| window_size[COL] < 100/* to update later */) {
        mvprintw(0, 0,"You need a bigger window to play!!"); // to be centered later
        goto GAME_END;
    }

    // load last session, idk (or maybe only load it only if continue ? ) 
    

    int menu_input = main_menu(window_size);
    MAIN_MENU:
    //wait for input with ncurses or smth
    switch(menu_input) {
        case NEW_GAME:

            reset_game_session(); //which will pick new word
            run_session();
            break;
        case CONTINUE:
            //continue session without change
            run_session();
            break;
        case SETTINGS:
            settings_menu();//to be implemented later
            break;
        case QUIT:
            goto GAME_END;
            break;
        //no need for default since do nothing! 
    }
    GAME_END:


    exit_prompt();
    
    getch(); // temporary
    curs_set(1); // to set back the terminal cursor
    endwin(); // to end the ncurses mode
}

//MARK: END






//MARK: DEFINITIONS
int main_menu(int window_size[2]) {

}

void settings_menu() { // for later

}

void exit_prompt() {

}


//MARK:gameplay
void run_session() {
    
    char wordle_guess[6] = {"     "};//spaces means empty
    int key_stroke, number_of_entered_letters = 0;//rename to something less verbose
    //works with a game session that already exists
    //display UI;
    while(game_session.number_of_used_attempts < 6) {
        //either display cursor at (game_session.cursor[X],game_session.cursor[Y])
        //set it to the start of the line 
        //display cursor position
        GET_VALID_WORD:
        while(1) {
            //take input
            
            key_stroke;
            switch(key_stroke) {
                case LEFT_KEY:
                    if(game_session.cursor_position[X] != FIRST_CELL_INDEX) {
                        game_session.cursor_position[X] += LEFT;
                        //update UI cursor position
                    }//else do nothing
                    break;
                case RIGHT_KEY:
                    if(game_session.cursor_position[X] != LAST_CELL_INDEX) {
                        game_session.cursor_position[X] += RIGHT;
                        //update UI cursor position
                    }
                    break;
                case ENTER_KEY:
                    if(number_of_entered_letters < 5) {
                        // display too short 
                    } else {
                        if(strcmp(game_session.wordle_answer, wordle_guess) == 0) {
                            //you win
                            goto SESSION_END;
                        }
                        else if(is_valid_word(wordle_guess)) {
                            //update the UI aka
                            //color the letters apporpriately
                            //update session_history matrix; game_session.history_matrix[][6];
                            //only reaches here if word is valid but not correct so we used an attempt 
                            ++game_session.number_of_used_attempts;

                            //prepare cursor for next attempt (if there is one)
                            game_session.cursor_position[Y]+= DOWN;
                            game_session.cursor_position[X] = FIRST_CELL_INDEX;
                            //NOTE: DO NOT display position, because it will show outside
                            //      it is already updated at the start of every attempt
                            goto NEXT_ATTEMPT;
                        } else {//INVALID WORD
                            //display warning word not found 
                            break; //exit the switch case and wait for new input
                        }
                    }
                case BACK_SPACE_KEY:
                    //NOTE:  wordle_guess[game_session.cursor_position[X]] is basically current cell pointed by cursor
                    //        and see if it's not space aka not empty, i decided that ' ' == empty
                    if(game_session.cursor_position[X] == FIRST_CELL_INDEX || wordle_guess[game_session.cursor_position[X]] != ' ') {
                        wordle_guess[game_session.cursor_position[X]] = ' ';
                        //update UI
                    } else {//not first and is empty so we move cursor back and delete it
                        --game_session.cursor_position[X];//move back
                        wordle_guess[game_session.cursor_position[X]] = ' ';
                        //update UI
                    }
                    break;
                default: //no we have to handle other letters
                    if(isLetter(key_stroke)) {//writing behavior , do whatver with isLetter whether it's a function or it's inside here
                        if(wordle_guess[game_session.cursor_position[X]] == ' ') {
                            //take input and update current wordle_guess
                            if(game_session.cursor_position[X] != LAST_CELL_INDEX) {
                                ++game_session.cursor_position[X];
                            }
                            //update UI
                        }
                    } //ignore anything else
                    break;
            }
        }//END of GET VALID WORD
        
        NEXT_ATTEMPT://go here so we go to next attempt
    }
    
    SESSION_END:
    //we either run out of attemps or we won the game

}

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