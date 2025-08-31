#include <ncurses.h>
#include <unistd.h>
#include "header.h"

void highlight_letter(Game_Session *game_session, int color, int letter_position);

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
