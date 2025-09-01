#include <ncurses.h>
#include <unistd.h>
#include "header.h"

void highlight_letter(Game_Session *game_session, int color, int letter_position);

void spiral_clearing_animation(int menu_start_col, int menu_start_row) {
    if(menu_start_row == 0 || menu_start_col == 0) return;
    int top_row, right_col, bottom_row, left_col;
    //initialisation
    //int top           , int left          , int bottom                    , int right)
    //menu_start_row + 1, menu_start_col + 2, menu_start_row + MENU_HEIGHT-2, menu_start_col + MENU_WIDTH - 3
    top_row = menu_start_row + 1;//skip the top border
    left_col = menu_start_col + 2;//skip the left col border
    bottom_row = menu_start_row + MENU_HEIGHT - 2;//goes back before bottom row, because menuStart + menu height - 1 is last row, so -1 to go back 1 more time
    right_col = menu_start_col + MENU_WIDTH - 3;//goes back by 3, because menuStart + MENU_WIDTH - 1 is right col edge, so go back by 2 characters because right col is 2 characters thick
    // spiral algorithm by Haytam
    int speed = 1200;
    while (top_row <= bottom_row && left_col <= right_col) {
        // top_row 5 rows
        for (int i = left_col; i <= right_col; i++) {
            for (int j = 0; j < 5; ++j)
                mvaddch(top_row + j, i, ' ');
            refresh();
            usleep(speed);
        }
        top_row += 5;

        // right_col 10 columns
        for (int i = top_row; i <= bottom_row; i++) {
            for (int j = 0; j < 10; ++j)
                mvaddch(i, right_col - j, ' ');

            refresh();
            usleep(speed);
        }
        right_col -= 10;

        // bottom_row  5 rows
        if (top_row <= bottom_row) {
            for (int i = right_col; i >= left_col; i--) {
                for (int j = 0; j < 5; ++j)
                    mvaddch(bottom_row - j, i, ' ');
                refresh();
                usleep(speed);
            }
            bottom_row -= 5;
        }

        // left_col 10 columns
        if (left_col <= right_col) {
            for (int i = bottom_row; i >= top_row; i--) {
                for (int j = 0; j < 10; ++j)
                    mvaddch(i, left_col + j, ' ');
                refresh();
                usleep(speed);
            }
            left_col += 10;
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
