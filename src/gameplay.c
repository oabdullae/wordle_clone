#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "header.h"

int pick_random_word(char *buffer);
void change_cursor(Game_Session *game_session, int action);
void print_ascii_letter(Game_Session *game_session, Ascii_Art_Letter letters_vector[26], char letter);
void delete_ascii_letter(Game_Session *game_session);
bool is_word_english(char* word)
// void move_cursor(Game_Session *game_session, int attempt, int old_cursor, int new_cursor);

void reset_game_session(Game_Session *game_session, int window_size[2]) {
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
    game_session->menu_start_row = (window_size[ROW] - MENU_HEIGHT)/2;
    game_session->menu_start_col = (window_size[COL] - MENU_WIDTH)/2;
    pick_random_word(game_session->wordle_answer);
    /* temporary debug */mvprintw(0, 0, "randomly picked word: %s", game_session->wordle_answer);
}

void run_session(Game_Session *game_session, Ascii_Art_Letter letters_vector[26]) { // skeleton made by Anas
    char wordle_guess[6] = {"     "}; //spaces means empty
    int key_stroke/* , number_of_entered_letters = 0 */;//rename to something less verbose
    //works with a game session that already exists
    //display UI;
    while (game_session->current_attempt < NO_ATTEMPTS) { // session loop consisting of at most 6 attempts
        //either display cursor at (game_session.cursor[X],game_session.cursor[Y])
        //set it to the start of the line 
        game_session->cursor = 0;

        //display cursor position
        usleep(150000);
        change_cursor(game_session, PRINT_CURSOR);

        while (1) { // GET_VALID_WORD loop
            // take input
            int key_stroke = getch();
            switch (key_stroke) {
                case KEY_LEFT:
                    if(game_session->cursor != FIRST_CELL_INDEX) {
                        // update cursor, and its UI cursor position
                        change_cursor(game_session, DELETE_CURSOR);
                        game_session->cursor += LEFT;
                        change_cursor(game_session, PRINT_CURSOR);

                    }//else do nothing
                    break;
                
                case KEY_RIGHT:
                    if(game_session->cursor != LAST_CELL_INDEX) {
                        // update cursor, and its UI cursor position
                        change_cursor(game_session,  DELETE_CURSOR);
                        game_session->cursor += RIGHT;
                        change_cursor(game_session, PRINT_CURSOR);

                    }//else do nothing
                    break;
                case '\n':
                    if(game_session->entered_letters < 5) {
                        // display too short 
                    } else {
                        if(strcmp(game_session->wordle_answer, game_session->history_matrix[game_session->current_attempt]) == 0) {
                            //you win update ui
                            goto SESSION_END;
                        }
                        else if(is_word_english(game_session->history_matrix[game_session->current_attempt])) {
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
                    if(game_session->cursor != FIRST_CELL_INDEX && game_session->history_matrix[game_session->current_attempt][game_session->cursor] == ' ') {
                        // move cursor backwards 
                        change_cursor(game_session, DELETE_CURSOR);
                        game_session->cursor += LEFT;
                        change_cursor(game_session, PRINT_CURSOR);


                    }
                    // if cursor got moved backwards we delete the letter that was before the cursor
                    // if not, then we just delete the letter that the cursor is pointing at, and keep cursor at its position
                    game_session->history_matrix[game_session->current_attempt][game_session->cursor] = ' ';
                        
                    // delete letter in UI
                    delete_ascii_letter(game_session); // TODO

                    // /* debug */ mvprintw(game_session->menu_start_row + 2  + game_session->current_attempt * (CELL_HEIGHT + 1),
                    //                         game_session->menu_start_col + 3 + game_session->cursor * (CELL_WIDTH + 2),
                    //                         " "
                    //                     ); // kept for history

                    // decrement entered_letters counter
                    game_session->entered_letters--;
                    break;

                default:
                    if (isalpha(key_stroke)) { //writing behavior , do whatver with isLetter whether it's a function or it's inside here
                        if (game_session->history_matrix[game_session->current_attempt][game_session->cursor] == ' ') { // if cell at cursor is empty
                            // update history_matrix
                            game_session->history_matrix[game_session->current_attempt][game_session->cursor] = tolower(key_stroke);

                            //update UI
                            // /* debug */ mvprintw(game_session->menu_start_row + 2  + game_session->current_attempt * (CELL_HEIGHT + 1),
                            //                      game_session->menu_start_col + 3 + game_session->cursor * (CELL_WIDTH + 2),
                            //                      "%c",
                            //                      toupper(key_stroke)
                            //             ); // kept for history
                            print_ascii_letter(game_session, letters_vector, tolower(key_stroke)); // TODO

                            // pushing cursor forward if it's not alread at the end
                            if (game_session->cursor != LAST_CELL_INDEX) {
                                change_cursor(game_session, DELETE_CURSOR);
                                game_session->cursor += RIGHT;
                                change_cursor(game_session, PRINT_CURSOR);
                            }

                            // increment the entered_letters
                            game_session->entered_letters++;
                        }
                        // TODO: later
                        // else { // warning: cell is already filled

                        // }
                    } // else ignore anything else
                    break;

            }
        }//END of GET VALID WORD
        
        // dont forget to increment game_session->current_attempt 
        // dont forget to clear the cursor when the attempt is over

        // NEXT_ATTEMPT://go here so we go to next attempt

    }
    
    // SESSION_END:
    //we either run out of attemps or we won the game

}