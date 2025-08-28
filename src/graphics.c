#include "header.h"
#include <ncurses.h>

void init_game_colors() {
    init_pair(BLUE, COLOR_BLACK, COLOR_BLUE);
    init_color(10,53, 255, 53); // green
    init_color(8, 500, 500, 500); // gray
    init_color(9, 228, 255, 70); // yellow
}

void change_cursor(Game_Session *game_session, int action) {
    attron((action == PRINT_CURSOR) ? (COLOR_PAIR(BLUE)) : A_STANDOUT); // cursor will be blue
    
    int absolute_col_start = game_session->menu_start_col + 2; // we always have to start at menu_start_col and add 2 to skip the border
    int col_shift_amount = game_session->cursor * (CELL_WIDTH + 2); // cell_width + border scaled by where the cursor, if cursor=0 then we dont shift and start at absolute_col_start
    int cell_base_start_col = absolute_col_start + col_shift_amount; // we increment the absolute_col_start with col_shift_amount that gets scaled by the cursor
    
    int absolute_row_start = game_session->menu_start_row + CELL_HEIGHT + 1;
    int row_shift_amount = game_session->current_attempt * (CELL_HEIGHT + 1);
    
    for (int i = 0; i < CELL_WIDTH; ++i) {
        mvaddch(absolute_row_start + row_shift_amount, i + cell_base_start_col, ' ');
    }
    
    attroff((action == DELETE_CURSOR) ? A_STANDOUT : (COLOR_PAIR(BLUE)));
    refresh();
}

/*
void highlight_letter(Game_Session *game_session, int color, int index, int attempt, int window_size[2]) {
    int cell_width = 18;
    int cell_height = 8;
    int menu_width = 102;
    int menu_height = 55;

    // Setup color
    if(color == 8){
        init_pair(color, COLOR_WHITE, 8);
    }
    if(color == 9){
        init_pair(color, COLOR_WHITE, 9);
    }
    if(color == 10){
        init_pair(color, COLOR_WHITE, 10);
    }

    // Menu top-left corner
    int menu_start_col = (window_size[COL] - menu_width) / 2;
    int menu_start_row = (window_size[ROW] - menu_height) / 2;

    // Cell top-left corner (inside grid)
    int letter_row = menu_start_row + (1 + cell_height) * attempt + 1;
    int letter_col = menu_start_col + (2 + cell_width) * index + 2;
    game_session->history_matrix[]
    
    // Loop over the cell area
    for (int i = 0; i < cell_height; i++) {
        mvprintw(letter_row + i, letter_col, "HHHHHHHHHHHHHHHHH ");
        attron(COLOR_PAIR(color));
        mvprintw(letter_row + i, letter_col, "%s", game_session->history_matrix[letter_row + i][menu_start_row + (1 + CELL_HEIGHT)*attempt ]);//matrix only take letter normal way
        attroff(COLOR_PAIR(color));
    }

    refresh();
}
*/