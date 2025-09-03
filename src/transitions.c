#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include "header.h"

void highlight_letter(Game_Session *game_session, int color, int letter_position);
void session_summary(Game_Session *game_session, int top_border, int left_border);

void spiral_clearing_animation(int top, int left, int bottom, int right) {
    // spiral algorithm by Haytam
    int speed = 1200;
    while (top <= bottom && left <= right) {
        // top 5 rows
        for (int i = left; i <= right; i++) {
            for (int j = 0; j < 5; ++j)
                mvaddch(top + j, i, ' ');
            refresh();
            usleep(speed);
        }
        top += 5;

        // right 10 columns
        for (int i = top; i <= bottom; i++) {
            for (int j = 0; j < 10; ++j)
                mvaddch(i, right - j, ' ');

            refresh();
            usleep(speed);
        }
        right -= 10;

        // bottom  5 rows
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                for (int j = 0; j < 5; ++j)
                    mvaddch(bottom - j, i, ' ');
                refresh();
                usleep(speed);
            }
            bottom -= 5;
        }

        // left 10 columns
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                for (int j = 0; j < 10; ++j)
                    mvaddch(i, left + j, ' ');
                refresh();
                usleep(speed);
            }
            left += 10;
        }
    }
}

void cell_grid_animation(int top, int left, int bottom, int right, int window_size[2]) {
    int speed = 10000;
    int menu_start_col = (window_size[COL] - MENU_WIDTH)/2;
    int menu_start_row = (window_size[ROW] - MENU_HEIGHT)/2; // they start with borders included btw
    attron(A_STANDOUT);
    for (int i = top, k = 0; k < MENU_WIDTH-2 || i <= bottom ; i++, k++) {
        if (i <= bottom) {
            // the 2 left internal border-columns of the grid get drawn from top to bottom
            mvprintw(i, menu_start_col + (2 + CELL_WIDTH)*1, "  "); // the added 2 is the size of the column-border
            mvprintw(i, menu_start_col + (2 + CELL_WIDTH)*2, "  ");
            
            // the 2 right internal border-colums of the grid get drawn from bottom to top
            mvprintw(bottom - k, menu_start_col + (2 + CELL_WIDTH)*3, "  ");
            mvprintw(bottom - k, menu_start_col + (2 + CELL_WIDTH)*4, "  ");
        }

        if (k < MENU_WIDTH-2) {
            // the 2 top internal border-rows of the grid get drawn from left to right
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*1 , left + k, ' '); // the added 1 is the size of the row-border
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*2 , left + k , ' ');

            // the 2 bottom internal border-rows of the grid get drawn from right to left
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*4, right - k, ' ');
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*5, right - k, ' ');
            
            //the midlle internal border-row of the grid gets drwan from the middle to the sides
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*3, left + ((right - left)/2) + (k/2), ' '); // right - left is just the width
            mvaddch(menu_start_row + (1 + CELL_HEIGHT)*3, left + ((right - left)/2 )- (k/2), ' ');
        }
        refresh();
        flushinp(); // discard queueed input
        usleep(speed);
    }
    attroff(A_STANDOUT);
}

void invalid_word_warning(Game_Session *game_session) {
    for (int j = 0; j < 2; ++j) {
        attron(A_BLINK);
        for (int i = 0; i < WORD_LENGTH; ++i) {
            highlight_letter(game_session, RED, i);
        }
        attroff(A_BLINK);
        flushinp();
        usleep(200000);
        for (int i = 0; i < WORD_LENGTH; ++i) {
            highlight_letter(game_session, NO_COLOR, i);
        }
        flushinp(); // discard queueed input
        usleep(200000);
    }
}

void too_short_warning(Game_Session *game_session) {
    for (int j = 0; j < 2; ++j) {
        attron(A_BLINK);
        for (int i = 0; i < WORD_LENGTH; ++i) {
            if (game_session->history_matrix[game_session->current_attempt][i] == ' ') // only flash empty cells
                highlight_letter(game_session, RED, i);
        }
        attroff(A_BLINK);
        flushinp();
        usleep(200000);
        for (int i = 0; i < WORD_LENGTH; ++i) {
            if (game_session->history_matrix[game_session->current_attempt][i] == ' ') // only unflash empty cells
                highlight_letter(game_session, NO_COLOR, i);
        }
        flushinp(); // discard queueed input
        usleep(200000);
    }
}

void winning_animation(Game_Session *game_session) {
    int winning_top_border = game_session->menu_start_row+1 /* + CELL_HEIGHT */;
    int winning_left_border = game_session->menu_start_col+2 + CELL_WIDTH; // skip a cell and start
    int winning_animation_width = CELL_WIDTH*3 + 4; // +4 column borders
    int options_left_border = game_session->menu_start_col+2 + (CELL_WIDTH+2)*2; // skip 2 cells and start
    int options_width = CELL_WIDTH*3 + 2; // take 3 cells of width

    
    spiral_clearing_animation(game_session->menu_start_row+1, game_session->menu_start_col+2, game_session->menu_start_row+MENU_HEIGHT-2, game_session->menu_start_col+MENU_WIDTH-3);
    
    // for (int i = 0; i < MENU_WIDTH; ++i) {
    //     mvprintw(20, game_session->menu_start_col+i-1, "%d", i%10);
    // }

    // mvprintw(++winning_top_border, winning_left_border + (winning_animation_width - 10)/2, "YOU WON! %d", winning_animation_width);
    
    // return;
    // attron(COLOR_PAIR(GREEN));
    // for (int i = 3; i >= 0; --i) {
    // int winning_top_border = game_session->menu_start_row+1 + CELL_HEIGHT;

    // YOU WON animated (just simply now, we may make it dance later :P)
    mvprintw(++winning_top_border, winning_left_border+2 /* idk why +2 it just centered it */ + (winning_animation_width - 58)/2 , "         8b        d8   ,ad8888ba,   88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "          Y8,    ,8P   d8\"'    `\"8b  88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "           Y8,  ,8P   d8'        `8b 88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "            \"8aa8\"    88          88 88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "             `88'     88          88 88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "              88      Y8,        ,8P 88        88         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "              88       Y8a.    .a8P  Y8a.    .a8P         "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "              88        `\"Y8888Y\"'    `\"Y8888Y\"'          "); refresh(); usleep(20000);
    ++winning_top_border;
    for (int i = 0; i < winning_animation_width; ++i) {
        mvaddch(winning_top_border, winning_left_border+2+i, ' ');
    }
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "    I8,            ,8I   ,ad8888ba,   888b      88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "    `8b            d8'  d8\"'    `\"8b  8888b     88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "     \"8,          ,8\"  d8'        `8b 88 `8b    88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "      Y8    db    8P   88          88 88  `8b   88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "      `8b   aa   d8'   88          88 88   `8b  88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "       `8a  88  a8'    Y8,        ,8P 88    `8b 88  88    "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "        `8a'  'a8'      Y8a.    .a8P  88     `8888        "); refresh(); usleep(20000);
    mvprintw(++winning_top_border, winning_left_border+2 + (winning_animation_width - 58)/2, "         `8'  '8'        `\"Y8888Y\"'   88      `888  88    "); refresh(); usleep(20000);

    ++winning_top_border;
    // session summary 
    session_summary(game_session, winning_top_border, game_session->menu_start_col + 2);


    // winning_menu to be moved to menus.c    
    // NEW GAME
    winning_top_border += 5;
    mvprintw(++winning_top_border, options_left_border + (options_width - 28)/2, "     __       __  _       __");
    mvprintw(++winning_top_border, options_left_border + (options_width - 28)/2, "|\\| |_  | |  /__ |_| |V| |_ ");
    mvprintw(++winning_top_border, options_left_border + (options_width - 28)/2, "| | |__ |^|  \\_| | | | | |__");
    winning_top_border += 2;
    attron(A_STANDOUT | A_BLINK);
    for (int i = 0; i < 32; ++i) {
        mvaddch(winning_top_border, options_left_border + (options_width - 32)/2 + i, '=');
    }
    attroff(A_STANDOUT | A_BLINK);

    // MAIN MENU
    ++winning_top_border;
    mvprintw(++winning_top_border, options_left_border +(options_width - 32)/2, "     _  ___           __        ");
    mvprintw(++winning_top_border, options_left_border +(options_width - 32)/2, "|V| |_|  |  |\\|  |V| |_  |\\| | |");
    mvprintw(++winning_top_border, options_left_border +(options_width - 32)/2, "| | | | _|_ | |  | | |__ | | |_|");
    winning_top_border += 2;
    attron(A_STANDOUT | A_BLINK);
    for (int i = 0; i < 32; ++i) {
        mvaddch(winning_top_border, options_left_border + (options_width - 32)/2 + i, '=');
    }
    attroff(A_STANDOUT | A_BLINK);

    // QUIT
    ++winning_top_border;
    mvprintw(++winning_top_border, options_left_border +(options_width - 16)/2, " _      ___ ___");
    mvprintw(++winning_top_border, options_left_border +(options_width - 16)/2, "/ \\ | |  |   | ");
    mvprintw(++winning_top_border, options_left_border +(options_width - 16)/2, "\\_X |_| _|_  | ");
    winning_top_border += 2;
    attron(A_STANDOUT | A_BLINK);
    for (int i = 0; i < 32; ++i) {
        mvaddch(winning_top_border, options_left_border + (options_width - 32)/2 + i, '=');
    }
    attron(A_STANDOUT | A_BLINK);

    
    // refresh();
    // usleep(200000);
    // for (int i = 0; i < MENU_HEIGHT- (1+1) /* remove both borders left and right */; ++i) {
    //     for (int j = 0; j < MENU_WIDTH-(2+2); ++j) {
    //         mvaddch(game_session->menu_start_row+1+i, game_session->menu_start_col+2+j, ' ');
    //     }
    // }
    // }
    // attroff(COLOR_PAIR(GREEN));
}

/* session_summary: used in winning and losing screen */
void session_summary(Game_Session *game_session, int top_border, int left_border) {
    int summary_width = (CELL_WIDTH * 2) + 2; // +2 is the middle border between the two cells
    mvprintw(++top_border, left_border + (summary_width - 15) / 2, " __  _       __");
    mvprintw(++top_border, left_border + (summary_width - 15) / 2, "/__ |_| |V| |_ ");
    mvprintw(++top_border, left_border + (summary_width - 15) / 2, "\\_| | | | | |__");
    ++top_border;
    mvprintw(++top_border, left_border + (summary_width - 27) / 2, " __              _   _     ");
    mvprintw(++top_border, left_border + (summary_width - 27) / 2, "(_  | | |V| |V| |_| |_) \\_/");
    mvprintw(++top_border, left_border + (summary_width - 27) / 2, "__) |_| | | | | | | | \\  | ");

    int overview_table_top = top_border + 3;
    int overview_table_left = left_border + 4;
    for (int i = 0; i < NUM_ATTEMPTS; ++i) { // loop over 6 row-attempts
        for (int j = 0; j < WORD_LENGTH; ++j) { // loop over 5 letters per attempt
            attron(COLOR_PAIR(game_session->matrix_colors[i][j]));
            top_border = overview_table_top;
            mvprintw(overview_table_top  + (i*4), overview_table_left + j*5, "     ");
            mvprintw(overview_table_top+1 +(i*4), overview_table_left + j*5, "  %c  ", (game_session->history_matrix[i][j] != ' ') ? toupper(game_session->history_matrix[i][j]) : '_');
            mvprintw(overview_table_top+2 + (i*4), overview_table_left + j*5, "     ");
            attroff(COLOR_PAIR(game_session->matrix_colors[i][j]));
            overview_table_left += 2;
        }
        overview_table_left = left_border + 4; // reset the overview_table_left to its initial value
    }
}

void correct_word_animation(Game_Session *game_session) {
    for (int i = 0; i < WORD_LENGTH+2; ++i) {
        if (i < WORD_LENGTH) {
            highlight_letter(game_session, GREEN, i);
            game_session->matrix_colors[game_session->current_attempt][i] = GREEN;
        }
        if (i-1 >= 0)
            highlight_letter(game_session, NO_COLOR, i-1);
        if (i-2 >= 0)
            highlight_letter(game_session, GREEN, i-2);
        usleep(200000);
    }
}