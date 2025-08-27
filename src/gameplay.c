#include <ncurses.h>
#include <stdbool.h>
#include "header.h"

int pick_random_word(char *buffer);

void reset_game_session(Game_Session *game_session) {
    for (int i = 0; i < NO_ATTEMPTS; ++i) { // 6 attempts
        for (int j = 0; j < WORD_LENGTH; ++j) { // 5 characters 
            game_session->history_matrix[i][j] = ' ';
            game_session->matrix_colors[i][j] = NO_COLOR;
        }
        game_session->history_matrix[i][WORD_LENGTH] = '\0';
    }
    game_session->current_attempt = 0;
    game_session->cursor = 0;
    game_session->game_ended = false;
    game_session->entered_letters = 0;
    game_session->time_elapsed = 0;
    pick_random_word(game_session->wordle_answer);
    /* temporary debug */mvprintw(0, 0, "randomly picked word: %s", game_session->wordle_answer);
}
#if 0
void run_session() {
    char wordle_guess[6] = {"     "}; //spaces means empty
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
            
            int key_stroke;
            switch(key_stroke) {
                case KEY_LEFT:
                    if(game_session.cursor_position[X] != FIRST_CELL_INDEX) {
                        game_session.cursor_position[X] += LEFT;
                        //update UI cursor position
                    }//else do nothing
                    break;
                case KEY_RIGHT:
                    if(game_session.cursor_position[X] != LAST_CELL_INDEX) {
                        game_session.cursor_position[X] += RIGHT;
                        //update UI cursor position
                    }
                    break;
                case '\n':
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
                case KEY_BACKSPACE:
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
#endif