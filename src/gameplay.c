#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "header.h"
#include <stdlib.h>

void spiral_clearing_animation(int menu_start_row, int menu_start_col);
int escape_menu(Game_Session* game_session);
void save_session(Game_Session *gamesession);
int pick_random_word(char *buffer);
void change_cursor(Game_Session *game_session, int action);
void print_ascii_letter(Game_Session *game_session, Ascii_Art_Letter letters_vector[26], char letter);
void delete_ascii_letter(Game_Session *game_session);
bool is_word_english(char* word);
void highlight_letter(Game_Session *game_session, int color, int letter_position);
void invalid_word_warning(Game_Session *game_session);
void too_short_warning(Game_Session *game_session);
void restore_after_pause(Game_Session *game_session, Ascii_Art_Letter letters_vector[26]);
// void move_cursor(Game_Session *game_session, int attempt, int old_cursor, int new_cursor);

void reset_game_session(Game_Session *game_session, int window_size[2]) {
    for (int i = 0; i < NUM_ATTEMPTS; ++i) { // 6 attempts
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
    // strcpy(game_session->wordle_answer, "thumb");
    /* temporary debug */mvprintw(0, 0, "randomly picked word: %s", game_session->wordle_answer);
}

void run_session(Game_Session *game_session, Ascii_Art_Letter letters_vector[26]) { // skeleton made by Anas
    // char wordle_guess[6] = {"     "}; //spaces means empty
    int key_stroke/* , number_of_entered_letters = 0 */;//rename to something less verbose
    //works with a game session that already exists
    //display UI;
    // /* debug */int index = 2;
    while (game_session->current_attempt < NUM_ATTEMPTS) { // session loop consisting of at most 6 attempts
        //either display cursor at (game_session.cursor[X],game_session.cursor[Y])
        //set it to the start of the line 
        game_session->cursor = 0;

        //display cursor position
        usleep(150000);
        change_cursor(game_session, PRINT_CURSOR);
        
        bool is_valid_word;
        do { // GET_VALID_WORD loop, will loop while the word is invalid
            is_valid_word = false;
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
                
                    if (game_session->entered_letters < 5) {
                        // display too short
                        too_short_warning(game_session);
                    }
                    else {
                        if (strcmp(game_session->wordle_answer, game_session->history_matrix[game_session->current_attempt]) == 0) {
                            // color all letters in green
                            for (int i = 0; i < WORD_LENGTH; ++i) {
                                highlight_letter(game_session, GREEN, i);
                            }
                            
                            //you win update ui
                            // goto SESSION_END; // just make current_attempt = 6 to exit loop
                            game_session->current_attempt = 6;
                            is_valid_word = true;
                            refresh();
                            usleep(2000000);
                        }
                        else if (is_word_english(game_session->history_matrix[game_session->current_attempt])) {
                            is_valid_word = true;
                            //update the UI aka
                            //color the letters apporpriately
                            // colored_letters to hold the colored letters in the wordle answer
                            char colored_letters[5] = {' ', ' ', ' ', ' ', ' '}; // ' ' for uncolored, 'G' for green, 'Y' for yellow (we dont care about gray)
                            // we go green all correct letters in their positions first
                            for (int i = 0; i < WORD_LENGTH; ++i) { // to solve CANAL with CENAL
                                // check if the letter is in its position, if yes: green it and mark it already greened, and quit
                                if (game_session->history_matrix[game_session->current_attempt][i] == game_session->wordle_answer[i]) {
                                    // color green
                                    highlight_letter(game_session, GREEN, i);
                                    // marking green for this algorithm to work properly
                                    colored_letters[i] = 'G';
                                    // save for loading
                                    game_session->matrix_colors[game_session->current_attempt][i] = GREEN;
                                }
                            }
                            for (int i = 0; i < WORD_LENGTH; ++i) {
                                if (game_session->matrix_colors[game_session->current_attempt][i] == GREEN) // no need to check greened letters, just skip by continue;
                                    continue;
                                
                                // when not green , check all other letters that are not greened yet, if found color yellow and quit
                                for (int j = 0; j < WORD_LENGTH; ++j) {
                                    if (/* i != j && */
                                        colored_letters[j] != 'G' && /* solves THUMB with TOAST */
                                        colored_letters[j] != 'Y' /* solves LOGIN with HELLO and ALPHA with CANAL with the help of colored_letters[] */
                                    ) { // only search among letters that are uncolored, and of course skip the cas i==j, cuz we already checked that just above
                                        if (game_session->history_matrix[game_session->current_attempt][i] == game_session->wordle_answer[j]) {
                                            // color yellow
                                            highlight_letter(game_session, YELLOW, i);
                                            game_session->matrix_colors[game_session->current_attempt][i] = YELLOW;
                                            colored_letters[j] = 'Y'; // mark the letter yellowed for the algorithm to work properly
                                            break;
                                            
                                        }
                                    }
                                }
                                
                                
                                        /* old attempt, that can't yellow two repeated letters, it only yellows one (like in solution:LLAMA with guess:STALL)

                                        if (i != j && game_session->matrix_colors[game_session->current_attempt][j] != GREEN) { // i!=j cuz we already compared that before in trying to green it, 
                                                                                                                                // and the 2nd condition for comparing only with indices that are not greened yet, try THUMB as solution and TOAST as guess without skipping greened ones
                                            if (game_session->history_matrix[game_session->current_attempt][i] == game_session->wordle_answer[j]) { // compare if letter exists in the answer under the above conditions
                                                // here letter is found in answer, but before yellowing it we still need to 
                                                // check if letter was already yellowed, cuz if that was the case it should not be yellowed
                                                bool similar_yellowed_letter = false;
                                                for (int k = i - 1; k >= 0; --k) {
                                                    if (game_session->history_matrix[game_session->current_attempt][i] == game_session->history_matrix[game_session->current_attempt][k] && game_session->matrix_colors[game_session->current_attempt][k] == YELLOW) {
                                                        similar_yellowed_letter = true;
                                                        break;
                                                    }
                                                }
                                                if (similar_yellowed_letter == false) { // only yellow it if it was not already yellowed
                                                    // color yellow
                                                    highlight_letter(game_session, YELLOW, i);
                                                    game_session->matrix_colors[game_session->current_attempt][i] = YELLOW;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    */
                                // if no then gray it, this way no greened letter will appear yellow (e.g. if answer was THUMB, and guess was TOAST, only first T should be greened, and second T should not be yellowed)
                                if (game_session->matrix_colors[game_session->current_attempt][i] != YELLOW) { // if letter was not yellowed, gray it
                                    // color in gray
                                    highlight_letter(game_session, GRAY, i);
                                    game_session->matrix_colors[game_session->current_attempt][i] = GRAY;
                                }
                            }

                            /* OLD BUGGY ATTEMPT, but good trial that helped develop the above algorithm
                            for (int i = 0; i < WORD_LENGTH; ++i) {
                                // solve here;
                                char *ptr_found_letter = strchr(game_session->wordle_answer, game_session->history_matrix[game_session->current_attempt][i]);
                                if (ptr_found_letter == NULL) { // letter not found
                                    // color in gray
                                    highlight_letter(game_session, GRAY, i);
                                    game_session->matrix_colors[game_session->current_attempt][i] = GRAY;
                                }
                                else { // letter found
                                    int index_found_letter = ptr_found_letter - game_session->wordle_answer; // index where the letter is in the wordle_answer
                                    if (index_found_letter == i) { // the i is the index of the letter in the guess, here we ask if the position in guess is the same as in answer
                                        // color green
                                        highlight_letter(game_session, GREEN, i);
                                        game_session->matrix_colors[game_session->current_attempt][i] = GREEN;
                                    }
                                    else { // letter exists but not in its correct position
                                        for (int j = i-1; j >= 0; --j) { // this is done for letters that are already colored green, cuz they should not appear again in yellow, but in gray
                                            if (game_session->history_matrix[game_session->current_attempt][i] == game_session->history_matrix[game_session->current_attempt][j]) {
                                                if (game_session->matrix_colors[game_session->current_attempt][j] == GREEN) {
                                                    highlight_letter(game_session, GRAY, i);
                                                    game_session->matrix_colors[game_session->current_attempt][i] = GRAY;
                                                    break;
                                                }
                                                else { // letter was never colore green before
                                                    // color yellow
                                                    highlight_letter(game_session, YELLOW, i);
                                                    game_session->matrix_colors[game_session->current_attempt][i] = YELLOW;
                                                    break;
                                                }
                                            }
                                            else {
                                                // color yellow
                                                highlight_letter(game_session, YELLOW, i);
                                                game_session->matrix_colors[game_session->current_attempt][i] = YELLOW;
                                            }
                                        }
                                        
                                    }
                                }
                            }
                            */

                            // delete cursor in UI
                            change_cursor(game_session, DELETE_CURSOR);
                            // increment current_attempt for next attempt (if there is one)
                            game_session->current_attempt += DOWN; // also moves cursor down
                            game_session->cursor = FIRST_CELL_INDEX; // move cursor to the start
                            game_session->entered_letters = 0; // reset entered_letters counter back to 0

                            
                            //NOTE: DO NOT display position, because it will show outside
                            //      it is already updated at the start of every attempt
                            // goto NEXT_ATTEMPT;
                        }
                        else {//INVALID WORD
                            //display warning word not found 
                            is_valid_word = false;
                            invalid_word_warning(game_session);
                            // break; //exit the switch case and wait for new input // ANAAAAAAAASSSSS!!!!!!! you caused a bug by this
                        }
                    }
                    break; // ANAAAAASSS break should be here!!!!!

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
                    if (game_session->entered_letters != 0) // never decrement the counter if it already reached 0
                        game_session->entered_letters--;
                    break;

                

                case ESCAPE_KEY: // Escape key
                    // reask the player if he wanna quit to menu or to stop game or continue
                    // scr_dump("screen_save.scr");
                    int action = escape_menu(game_session);
                    
                    if (action == RESUME_GAME) {
                        // resume
                        // scr_restore("screen_save.scr"); // should restore the cursor as well
                        // refresh();

                        // restoring manually
                        restore_after_pause(game_session, letters_vector);
                        break;
                    }
                    else { // MAIN_MENU and SAVE_QUIT
                        save_session(game_session);
                        if (action == MAIN_MENU) {
                            //go back to menu 
                            spiral_clearing_animation(game_session->menu_start_row, game_session->menu_start_col);
                            
                            // break out of both loops by breaking their conditions
                            is_valid_word = true;
                            game_session->current_attempt = NUM_ATTEMPTS;
                            break;
                        }
                        else { // SAVE_QUIT option quit the game 
                            spiral_clearing_animation(game_session->menu_start_row, game_session->menu_start_col);
                            curs_set(1); // to set back the terminal cursor
                            endwin();
                            exit(0);
                        }
                    }
                    
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
        } while (!is_valid_word);//END of GET VALID WORD
        
        // dont forget to increment game_session->current_attempt 
        // dont forget to clear the cursor when the attempt is over

        // NEXT_ATTEMPT://go here so we go to next attempt

    }
    
    // SESSION_END:
    //we either run out of attemps or we won the game

}