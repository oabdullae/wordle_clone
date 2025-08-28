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
