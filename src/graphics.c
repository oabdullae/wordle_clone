#include <ncurses.h>
#include <string.h>
#include "header.h"

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

void print_ascii_letter(Game_Session *game_session, Ascii_Art_Letter letters_vector[26], char letter) {
    for (int i = 0; i < CELL_HEIGHT; ++i) {
        mvprintw(game_session->menu_start_row+1  + game_session->current_attempt*(CELL_HEIGHT + 1) + i, // calculated like in change_cursor() above
                game_session->menu_start_col+2 + game_session->cursor*(CELL_WIDTH + 2), // calculated like in change_cursor() above
                "%s", letters_vector[letter - 'a'].buffer[i]
                );
    }
}

void delete_ascii_letter(Game_Session *game_session) {
    for (int i = 0; i < CELL_HEIGHT; ++i) {
        for (int j = 0; j < CELL_WIDTH; ++j) {
            mvaddch(game_session->menu_start_row+1 + game_session->current_attempt*(CELL_HEIGHT + 1) + i,
                    game_session->menu_start_col+2 + game_session->cursor*(CELL_WIDTH + 2) + j, ' ');
        }
    }
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

void init_ascii_art(Ascii_Art_Letter letters_vector[26]) {
    // switch (letters_vector[0]->font) {
    //     case UNIVERS:
            // A
            strcpy(letters_vector[0].buffer[0],  "        db        ");
            strcpy(letters_vector[0].buffer[1],  "       d88b       ");
            strcpy(letters_vector[0].buffer[2],  "      d8'`8b      ");
            strcpy(letters_vector[0].buffer[3],  "     d8'  `8b     ");
            strcpy(letters_vector[0].buffer[4],  "    d8YaaaaY8b    ");
            strcpy(letters_vector[0].buffer[5],  "   d8\"\"\"\"\"\"\"\"8b   ");
            strcpy(letters_vector[0].buffer[6],  "  d8'        `8b  ");
            strcpy(letters_vector[0].buffer[7],  " d8'          `8b ");

            // B
            strcpy(letters_vector[1].buffer[0],  "   888888888ba    ");
            strcpy(letters_vector[1].buffer[1],  "   88       \"8b   ");
            strcpy(letters_vector[1].buffer[2],  "   88       ,8P   ");
            strcpy(letters_vector[1].buffer[3],  "   88aaaaaaa8P'   ");
            strcpy(letters_vector[1].buffer[4],  "   88\"\"\"\"\"\"\"8b,   ");
            strcpy(letters_vector[1].buffer[5],  "   88       `8b   ");
            strcpy(letters_vector[1].buffer[6],  "   88       a8P   ");
            strcpy(letters_vector[1].buffer[7],  "   888888888P\"    ");

            // C
            strcpy(letters_vector[2].buffer[0], "    ,ad88888ba,   ");
            strcpy(letters_vector[2].buffer[1], "   d8\"'     `\"8b  ");
            strcpy(letters_vector[2].buffer[2], "  d8'             ");
            strcpy(letters_vector[2].buffer[3], "  88              ");
            strcpy(letters_vector[2].buffer[4], "  88              ");
            strcpy(letters_vector[2].buffer[5], "  Y8,             ");
            strcpy(letters_vector[2].buffer[6], "   Y8a.     .a8P  ");
            strcpy(letters_vector[2].buffer[7], "    `\"Y88888Y\"'   ");

            // D
            strcpy(letters_vector[3].buffer[0], "  888888888ba,    ");
            strcpy(letters_vector[3].buffer[1], "  88       `\"8b   ");
            strcpy(letters_vector[3].buffer[2], "  88         `8b  ");
            strcpy(letters_vector[3].buffer[3], "  88          88  ");
            strcpy(letters_vector[3].buffer[4], "  88          88  ");
            strcpy(letters_vector[3].buffer[5], "  88          8P  ");
            strcpy(letters_vector[3].buffer[6], "  88       .a8P   ");
            strcpy(letters_vector[3].buffer[7], "  888888888Y\"'    ");

            // E
            strcpy(letters_vector[4].buffer[0],  "   888888888888   ");
            strcpy(letters_vector[4].buffer[1],  "   88             ");
            strcpy(letters_vector[4].buffer[2],  "   88             ");
            strcpy(letters_vector[4].buffer[3],  "   88aaaaaa       ");
            strcpy(letters_vector[4].buffer[4],  "   88\"\"\"\"\"\"       ");
            strcpy(letters_vector[4].buffer[5],  "   88             ");
            strcpy(letters_vector[4].buffer[6],  "   88             ");
            strcpy(letters_vector[4].buffer[7],  "   888888888888   ");

            // F
            strcpy(letters_vector[5].buffer[0], "   888888888888   ");
            strcpy(letters_vector[5].buffer[1], "   88             ");
            strcpy(letters_vector[5].buffer[2], "   88             ");
            strcpy(letters_vector[5].buffer[3], "   88aaaaaa       ");
            strcpy(letters_vector[5].buffer[4], "   88\"\"\"\"\"\"       ");
            strcpy(letters_vector[5].buffer[5], "   88             ");
            strcpy(letters_vector[5].buffer[6], "   88             ");
            strcpy(letters_vector[5].buffer[7], "   88             ");

            // G
            strcpy(letters_vector[6].buffer[0], "    ,ad88888ba,   ");
            strcpy(letters_vector[6].buffer[1], "   d8\"'     `\"8b  ");
            strcpy(letters_vector[6].buffer[2], "  d8'             ");
            strcpy(letters_vector[6].buffer[3], "  88              ");
            strcpy(letters_vector[6].buffer[4], "  88       88888  ");
            strcpy(letters_vector[6].buffer[5], "  Y8,         88  ");
            strcpy(letters_vector[6].buffer[6], "   Y8a.     .a88  ");
            strcpy(letters_vector[6].buffer[7], "    `\"Y888888P\"   ");

            // H
            strcpy(letters_vector[7].buffer[0], "   88        88   ");
            strcpy(letters_vector[7].buffer[1], "   88        88   ");
            strcpy(letters_vector[7].buffer[2], "   88        88   ");
            strcpy(letters_vector[7].buffer[3], "   88aaaaaaaa88   ");
            strcpy(letters_vector[7].buffer[4], "   88\"\"\"\"\"\"\"\"88   ");
            strcpy(letters_vector[7].buffer[5], "   88        88   ");
            strcpy(letters_vector[7].buffer[6], "   88        88   ");
            strcpy(letters_vector[7].buffer[7], "   88        88   ");

            // I
            strcpy(letters_vector[8].buffer[0], "    8888888888    ");
            strcpy(letters_vector[8].buffer[1], "        88        ");
            strcpy(letters_vector[8].buffer[2], "        88        ");
            strcpy(letters_vector[8].buffer[3], "        88        ");
            strcpy(letters_vector[8].buffer[4], "        88        ");
            strcpy(letters_vector[8].buffer[5], "        88        ");
            strcpy(letters_vector[8].buffer[6], "        88        ");
            strcpy(letters_vector[8].buffer[7], "    8888888888    ");

            // J
            strcpy(letters_vector[9].buffer[0], "            88    ");
            strcpy(letters_vector[9].buffer[1], "            88    ");
            strcpy(letters_vector[9].buffer[2], "            88    ");
            strcpy(letters_vector[9].buffer[3], "            88    ");
            strcpy(letters_vector[9].buffer[4], "            88    ");
            strcpy(letters_vector[9].buffer[5], "            88    ");
            strcpy(letters_vector[9].buffer[6], "    88,   ,d88    ");
            strcpy(letters_vector[9].buffer[7], "     \"Y8888P\"     ");

            // K
            strcpy(letters_vector[10].buffer[0], "   88      a8P    ");
            strcpy(letters_vector[10].buffer[1], "   88    ,88'     ");
            strcpy(letters_vector[10].buffer[2], "   88  ,88\"       ");
            strcpy(letters_vector[10].buffer[3], "   88,d88'        ");
            strcpy(letters_vector[10].buffer[4], "   8888\"88,       ");
            strcpy(letters_vector[10].buffer[5], "   88P   Y8b      ");
            strcpy(letters_vector[10].buffer[6], "   88     \"88,    ");
            strcpy(letters_vector[10].buffer[7], "   88       Y8b   ");

            // L
            strcpy(letters_vector[11].buffer[0], "   88             ");
            strcpy(letters_vector[11].buffer[1], "   88             ");
            strcpy(letters_vector[11].buffer[2], "   88             ");
            strcpy(letters_vector[11].buffer[3], "   88             ");
            strcpy(letters_vector[11].buffer[4], "   88             ");
            strcpy(letters_vector[11].buffer[5], "   88             ");
            strcpy(letters_vector[11].buffer[6], "   88             ");
            strcpy(letters_vector[11].buffer[7], "   888888888888   ");

            // M
            strcpy(letters_vector[12].buffer[0], " 88b          d88 ");
            strcpy(letters_vector[12].buffer[1], " 888b        d888 ");
            strcpy(letters_vector[12].buffer[2], " 88`8b      d8'88 ");
            strcpy(letters_vector[12].buffer[3], " 88 `8b    d8' 88 ");
            strcpy(letters_vector[12].buffer[4], " 88  `8b  d8'  88 ");
            strcpy(letters_vector[12].buffer[5], " 88   `8bd8'   88 ");
            strcpy(letters_vector[12].buffer[6], " 88    `88'    88 ");
            strcpy(letters_vector[12].buffer[7], " 88     `'     88 ");

            // N
            strcpy(letters_vector[13].buffer[0], "   888b      88   ");
            strcpy(letters_vector[13].buffer[1], "   8888b     88   ");
            strcpy(letters_vector[13].buffer[2], "   88 `8b    88   ");
            strcpy(letters_vector[13].buffer[3], "   88  `8b   88   ");
            strcpy(letters_vector[13].buffer[4], "   88   `8b  88   ");
            strcpy(letters_vector[13].buffer[5], "   88    `8b 88   ");
            strcpy(letters_vector[13].buffer[6], "   88     `8888   ");
            strcpy(letters_vector[13].buffer[7], "   88      `888   ");

            // O
            strcpy(letters_vector[14].buffer[0], "    ,ad8888ba,    ");
            strcpy(letters_vector[14].buffer[1], "   d8\"'    `\"8b   ");
            strcpy(letters_vector[14].buffer[2], "  d8'        `8b  ");
            strcpy(letters_vector[14].buffer[3], "  88          88  ");
            strcpy(letters_vector[14].buffer[4], "  88          88  ");
            strcpy(letters_vector[14].buffer[5], "  Y8,        ,8P  ");
            strcpy(letters_vector[14].buffer[6], "   Y8a.    .a8P   ");
            strcpy(letters_vector[14].buffer[7], "    `\"Y8888Y\"'    ");

            // P
            strcpy(letters_vector[15].buffer[0], "   888888888ba    ");
            strcpy(letters_vector[15].buffer[1], "   88       \"8b   ");
            strcpy(letters_vector[15].buffer[2], "   88       ,8P   ");
            strcpy(letters_vector[15].buffer[3], "   88aaaaaaa8P'   ");
            strcpy(letters_vector[15].buffer[4], "   88\"\"\"\"\"\"\"'     ");
            strcpy(letters_vector[15].buffer[5], "   88             ");
            strcpy(letters_vector[15].buffer[6], "   88             ");
            strcpy(letters_vector[15].buffer[7], "   88             ");

            // Q
            strcpy(letters_vector[16].buffer[0], "    ,ad8888ba,    ");
            strcpy(letters_vector[16].buffer[1], "   d8\"'    `\"8b   ");
            strcpy(letters_vector[16].buffer[2], "  d8'        `8b  ");
            strcpy(letters_vector[16].buffer[3], "  88          88  ");
            strcpy(letters_vector[16].buffer[4], "  88          88  ");
            strcpy(letters_vector[16].buffer[5], "  Y8,    \"88,,8P  ");
            strcpy(letters_vector[16].buffer[6], "   Y8a.    Y88P   ");
            strcpy(letters_vector[16].buffer[7], "    `\"Y8888Y\"Y8a  ");

            // R
            strcpy(letters_vector[17].buffer[0], "   888888888ba    ");
            strcpy(letters_vector[17].buffer[1], "   88       \"8b   ");
            strcpy(letters_vector[17].buffer[2], "   88       ,8P   ");
            strcpy(letters_vector[17].buffer[3], "   88aaaaaaa8P'   ");
            strcpy(letters_vector[17].buffer[4], "   88\"\"\"\"\"88'     ");
            strcpy(letters_vector[17].buffer[5], "   88     `8b     ");
            strcpy(letters_vector[17].buffer[6], "   88      `8b    ");
            strcpy(letters_vector[17].buffer[7], "   88       `8b   ");

            // S
            strcpy(letters_vector[18].buffer[0], "    ad888888ba    ");
            strcpy(letters_vector[18].buffer[1], "   d8\"      \"8b   ");
            strcpy(letters_vector[18].buffer[2], "   Y8,            ");
            strcpy(letters_vector[18].buffer[3], "   `Y8aaaaaa,     ");
            strcpy(letters_vector[18].buffer[4], "     `\"\"\"\"\"\"8b,   ");
            strcpy(letters_vector[18].buffer[5], "            `8b   ");
            strcpy(letters_vector[18].buffer[6], "   Y8a      a8P   ");
            strcpy(letters_vector[18].buffer[7], "    \"Y888888P\"    ");

            // T
            strcpy(letters_vector[19].buffer[0], "   888888888888   ");
            strcpy(letters_vector[19].buffer[1], "        88        ");
            strcpy(letters_vector[19].buffer[2], "        88        ");
            strcpy(letters_vector[19].buffer[3], "        88        ");
            strcpy(letters_vector[19].buffer[4], "        88        ");
            strcpy(letters_vector[19].buffer[5], "        88        ");
            strcpy(letters_vector[19].buffer[6], "        88        ");
            strcpy(letters_vector[19].buffer[7], "        88        ");

            // U
            strcpy(letters_vector[20].buffer[0], "   88        88   ");
            strcpy(letters_vector[20].buffer[1], "   88        88   ");
            strcpy(letters_vector[20].buffer[2], "   88        88   ");
            strcpy(letters_vector[20].buffer[3], "   88        88   ");
            strcpy(letters_vector[20].buffer[4], "   88        88   ");
            strcpy(letters_vector[20].buffer[5], "   88        88   ");
            strcpy(letters_vector[20].buffer[6], "   Y8a.    .a8P   ");
            strcpy(letters_vector[20].buffer[7], "    `\"Y8888Y\"'    ");

            // V
            strcpy(letters_vector[21].buffer[0], " 8b            d8 ");
            strcpy(letters_vector[21].buffer[1], " `8b          d8' ");
            strcpy(letters_vector[21].buffer[2], "  `8b        d8'  ");
            strcpy(letters_vector[21].buffer[3], "   `8b      d8'   ");
            strcpy(letters_vector[21].buffer[4], "    `8b    d8'    ");
            strcpy(letters_vector[21].buffer[5], "     `8b  d8'     ");
            strcpy(letters_vector[21].buffer[6], "      `8888'      ");
            strcpy(letters_vector[21].buffer[7], "       `88'       ");

            // W
            strcpy(letters_vector[22].buffer[0], "I8,            ,8I");
            strcpy(letters_vector[22].buffer[1], "`8b            d8'");
            strcpy(letters_vector[22].buffer[2], " \"8,          ,8\" ");
            strcpy(letters_vector[22].buffer[3], "  Y8    db    8P  ");
            strcpy(letters_vector[22].buffer[4], "  `8b   aa   d8'  ");
            strcpy(letters_vector[22].buffer[5], "   `8a  88  a8'   ");
            strcpy(letters_vector[22].buffer[6], "    `8a'  'a8'    ");
            strcpy(letters_vector[22].buffer[7], "     `8'  '8'     ");

            // X
            strcpy(letters_vector[23].buffer[0], "   8b        d8   ");
            strcpy(letters_vector[23].buffer[1], "    Y8,    ,8P    ");
            strcpy(letters_vector[23].buffer[2], "     `8b  d8'     ");
            strcpy(letters_vector[23].buffer[3], "       Y88P       ");
            strcpy(letters_vector[23].buffer[4], "       d88b       ");
            strcpy(letters_vector[23].buffer[5], "     ,8P  Y8,     ");
            strcpy(letters_vector[23].buffer[6], "    d8'    `8b    ");
            strcpy(letters_vector[23].buffer[7], "   8P        Y8   ");

            // Y
            strcpy(letters_vector[24].buffer[0], "   8b        d8   ");
            strcpy(letters_vector[24].buffer[1], "    Y8,    ,8P    ");
            strcpy(letters_vector[24].buffer[2], "     Y8,  ,8P     ");
            strcpy(letters_vector[24].buffer[3], "      \"8aa 8\"     ");
            strcpy(letters_vector[24].buffer[4], "       `88'       ");
            strcpy(letters_vector[24].buffer[5], "        88        ");
            strcpy(letters_vector[24].buffer[6], "        88        ");
            strcpy(letters_vector[24].buffer[7], "        88        ");

            // Z
            strcpy(letters_vector[25].buffer[0],"   888888888888   ");
            strcpy(letters_vector[25].buffer[1],"            ,88   ");
            strcpy(letters_vector[25].buffer[2],"          ,88\"    ");
            strcpy(letters_vector[25].buffer[3],"        ,88\"      ");
            strcpy(letters_vector[25].buffer[4],"      ,88\"        ");
            strcpy(letters_vector[25].buffer[5],"    ,88\"          ");
            strcpy(letters_vector[25].buffer[6],"   88\"            ");
            strcpy(letters_vector[25].buffer[7],"   888888888888   ");

    //         break;
    //     case DOOM:

    //         break;
    //     case VARSITY:

    //         break;
    // }
}
