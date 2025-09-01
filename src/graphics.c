#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "header.h"

enum Letters{
    LETTER_A,
    LETTER_B,
    LETTER_C,
    LETTER_D,
    LETTER_E, 
    LETTER_F,
    LETTER_G,
    LETTER_H,
    LETTER_I,
    LETTER_J,
    LETTER_K,
    LETTER_L,
    LETTER_M,
    LETTER_N,
    LETTER_O,
    LETTER_P,
    LETTER_Q,
    LETTER_R,
    LETTER_S,
    LETTER_T,
    LETTER_U,
    LETTER_V,
    LETTER_W,
    LETTER_X,
    LETTER_Y,
    LETTER_Z,
};

void init_game_colors() {
    init_pair(NO_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLACK, COLOR_BLUE);
    init_color(GREEN, 53, 255, 53); // green
    init_pair(GREEN, COLOR_WHITE, GREEN);
    // init_color(GRAY, 160, 160, 150); // oabdullae's favorite gray 🥹
    init_color(GRAY, 128, 130, 115); // haytam's gray
    init_pair(GRAY, COLOR_WHITE, GRAY);
    init_color(YELLOW, 450, 450, 100); // yellow
    // init_color(YELLOW, 450, 450, 50); // yellow
    // init_color(YELLOW, 500, 500, 200); // yellow
    init_pair(YELLOW, COLOR_WHITE, YELLOW);
    init_pair(RED, COLOR_WHITE, COLOR_RED);



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


void highlight_letter(Game_Session *game_session, int color, int letter_position) {

    // Cell top-left corner (inside grid)
    int letter_start_col = game_session->menu_start_col+2 + (CELL_WIDTH + 2)*letter_position;
    int letter_start_row = game_session->menu_start_row+1 + (CELL_HEIGHT + 1)*game_session->current_attempt;

    // Loop over the cell area
    attron(COLOR_PAIR(color));
    for (int i = 0; i < CELL_HEIGHT; ++i) {
        for (int j = 0; j < CELL_WIDTH; ++j) {
            char c = mvinch(letter_start_row + i, letter_start_col + j) & A_CHARTEXT;
            mvaddch(letter_start_row + i, letter_start_col + j, c);
        }
    }
    attroff(COLOR_PAIR(color));

    refresh();
}


//TO automate this, make an ascii printer homework -> ncurses ascii printer -> integrate here
void init_ascii_art(Ascii_Art_Letter letters_vector[26]) {
    // switch (letters_vector[0]->font) {
    //     case UNIVERS:
            // A
            // A
            strcpy(letters_vector[LETTER_A].buffer[0],  "        db        ");
            strcpy(letters_vector[LETTER_A].buffer[1],  "       d88b       ");
            strcpy(letters_vector[LETTER_A].buffer[2],  "      d8'`8b      ");
            strcpy(letters_vector[LETTER_A].buffer[3],  "     d8'  `8b     ");
            strcpy(letters_vector[LETTER_A].buffer[4],  "    d8YaaaaY8b    ");
            strcpy(letters_vector[LETTER_A].buffer[5],  "   d8\"\"\"\"\"\"\"\"8b   ");
            strcpy(letters_vector[LETTER_A].buffer[6],  "  d8'        `8b  ");
            strcpy(letters_vector[LETTER_A].buffer[7],  " d8'          `8b ");

            // B
            strcpy(letters_vector[LETTER_B].buffer[0],  "   888888888ba    ");
            strcpy(letters_vector[LETTER_B].buffer[1],  "   88       \"8b   ");
            strcpy(letters_vector[LETTER_B].buffer[2],  "   88       ,8P   ");
            strcpy(letters_vector[LETTER_B].buffer[3],  "   88aaaaaaa8P'   ");
            strcpy(letters_vector[LETTER_B].buffer[4],  "   88\"\"\"\"\"\"\"8b,   ");
            strcpy(letters_vector[LETTER_B].buffer[5],  "   88       `8b   ");
            strcpy(letters_vector[LETTER_B].buffer[6],  "   88       a8P   ");
            strcpy(letters_vector[LETTER_B].buffer[7],  "   888888888P\"    ");

            // C
            strcpy(letters_vector[LETTER_C].buffer[0], "    ,ad88888ba,   ");
            strcpy(letters_vector[LETTER_C].buffer[1], "   d8\"'     `\"8b  ");
            strcpy(letters_vector[LETTER_C].buffer[2], "  d8'             ");
            strcpy(letters_vector[LETTER_C].buffer[3], "  88              ");
            strcpy(letters_vector[LETTER_C].buffer[4], "  88              ");
            strcpy(letters_vector[LETTER_C].buffer[5], "  Y8,             ");
            strcpy(letters_vector[LETTER_C].buffer[6], "   Y8a.     .a8P  ");
            strcpy(letters_vector[LETTER_C].buffer[7], "    `\"Y88888Y\"'   ");

            // D
            strcpy(letters_vector[LETTER_D].buffer[0], "  888888888ba,    ");
            strcpy(letters_vector[LETTER_D].buffer[1], "  88       `\"8b   ");
            strcpy(letters_vector[LETTER_D].buffer[2], "  88         `8b  ");
            strcpy(letters_vector[LETTER_D].buffer[3], "  88          88  ");
            strcpy(letters_vector[LETTER_D].buffer[4], "  88          88  ");
            strcpy(letters_vector[LETTER_D].buffer[5], "  88          8P  ");
            strcpy(letters_vector[LETTER_D].buffer[6], "  88       .a8P   ");
            strcpy(letters_vector[LETTER_D].buffer[7], "  888888888Y\"'    ");

            // E
            strcpy(letters_vector[LETTER_E].buffer[0],  "   888888888888   ");
            strcpy(letters_vector[LETTER_E].buffer[1],  "   88             ");
            strcpy(letters_vector[LETTER_E].buffer[2],  "   88             ");
            strcpy(letters_vector[LETTER_E].buffer[3],  "   88aaaaaa       ");
            strcpy(letters_vector[LETTER_E].buffer[4],  "   88\"\"\"\"\"\"       ");
            strcpy(letters_vector[LETTER_E].buffer[5],  "   88             ");
            strcpy(letters_vector[LETTER_E].buffer[6],  "   88             ");
            strcpy(letters_vector[LETTER_E].buffer[7],  "   888888888888   ");

            // F
            strcpy(letters_vector[LETTER_F].buffer[0], "   888888888888   ");
            strcpy(letters_vector[LETTER_F].buffer[1], "   88             ");
            strcpy(letters_vector[LETTER_F].buffer[2], "   88             ");
            strcpy(letters_vector[LETTER_F].buffer[3], "   88aaaaaa       ");
            strcpy(letters_vector[LETTER_F].buffer[4], "   88\"\"\"\"\"\"       ");
            strcpy(letters_vector[LETTER_F].buffer[5], "   88             ");
            strcpy(letters_vector[LETTER_F].buffer[6], "   88             ");
            strcpy(letters_vector[LETTER_F].buffer[7], "   88             ");

            // G
            strcpy(letters_vector[LETTER_G].buffer[0], "    ,ad88888ba,   ");
            strcpy(letters_vector[LETTER_G].buffer[1], "   d8\"'     `\"8b  ");
            strcpy(letters_vector[LETTER_G].buffer[2], "  d8'             ");
            strcpy(letters_vector[LETTER_G].buffer[3], "  88              ");
            strcpy(letters_vector[LETTER_G].buffer[4], "  88       88888  ");
            strcpy(letters_vector[LETTER_G].buffer[5], "  Y8,         88  ");
            strcpy(letters_vector[LETTER_G].buffer[6], "   Y8a.     .a88  ");
            strcpy(letters_vector[LETTER_G].buffer[7], "    `\"Y888888P\"   ");

            // H
            strcpy(letters_vector[LETTER_H].buffer[0], "   88        88   ");
            strcpy(letters_vector[LETTER_H].buffer[1], "   88        88   ");
            strcpy(letters_vector[LETTER_H].buffer[2], "   88        88   ");
            strcpy(letters_vector[LETTER_H].buffer[3], "   88aaaaaaaa88   ");
            strcpy(letters_vector[LETTER_H].buffer[4], "   88\"\"\"\"\"\"\"\"88   ");
            strcpy(letters_vector[LETTER_H].buffer[5], "   88        88   ");
            strcpy(letters_vector[LETTER_H].buffer[6], "   88        88   ");
            strcpy(letters_vector[LETTER_H].buffer[7], "   88        88   ");

            // I
            strcpy(letters_vector[LETTER_I].buffer[0], "    8888888888    ");
            strcpy(letters_vector[LETTER_I].buffer[1], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[2], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[3], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[4], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[5], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[6], "        88        ");
            strcpy(letters_vector[LETTER_I].buffer[7], "    8888888888    ");

            // J
            strcpy(letters_vector[LETTER_J].buffer[0], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[1], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[2], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[3], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[4], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[5], "            88    ");
            strcpy(letters_vector[LETTER_J].buffer[6], "    88,   ,d88    ");
            strcpy(letters_vector[LETTER_J].buffer[7], "     \"Y8888P\"     ");

            // K
            strcpy(letters_vector[LETTER_K].buffer[0], "   88      a8P    ");
            strcpy(letters_vector[LETTER_K].buffer[1], "   88    ,88'     ");
            strcpy(letters_vector[LETTER_K].buffer[2], "   88  ,88\"       ");
            strcpy(letters_vector[LETTER_K].buffer[3], "   88,d88'        ");
            strcpy(letters_vector[LETTER_K].buffer[4], "   8888\"88,       ");
            strcpy(letters_vector[LETTER_K].buffer[5], "   88P   Y8b      ");
            strcpy(letters_vector[LETTER_K].buffer[6], "   88     \"88,    ");
            strcpy(letters_vector[LETTER_K].buffer[7], "   88       Y8b   ");

            // L
            strcpy(letters_vector[LETTER_L].buffer[0], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[1], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[2], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[3], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[4], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[5], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[6], "   88             ");
            strcpy(letters_vector[LETTER_L].buffer[7], "   888888888888   ");

            // M
            strcpy(letters_vector[LETTER_M].buffer[0], " 88b          d88 ");
            strcpy(letters_vector[LETTER_M].buffer[1], " 888b        d888 ");
            strcpy(letters_vector[LETTER_M].buffer[2], " 88`8b      d8'88 ");
            strcpy(letters_vector[LETTER_M].buffer[3], " 88 `8b    d8' 88 ");
            strcpy(letters_vector[LETTER_M].buffer[4], " 88  `8b  d8'  88 ");
            strcpy(letters_vector[LETTER_M].buffer[5], " 88   `8bd8'   88 ");
            strcpy(letters_vector[LETTER_M].buffer[6], " 88    `88'    88 ");
            strcpy(letters_vector[LETTER_M].buffer[7], " 88     `'     88 ");

            // N
            strcpy(letters_vector[LETTER_N].buffer[0], "   888b      88   ");
            strcpy(letters_vector[LETTER_N].buffer[1], "   8888b     88   ");
            strcpy(letters_vector[LETTER_N].buffer[2], "   88 `8b    88   ");
            strcpy(letters_vector[LETTER_N].buffer[3], "   88  `8b   88   ");
            strcpy(letters_vector[LETTER_N].buffer[4], "   88   `8b  88   ");
            strcpy(letters_vector[LETTER_N].buffer[5], "   88    `8b 88   ");
            strcpy(letters_vector[LETTER_N].buffer[6], "   88     `8888   ");
            strcpy(letters_vector[LETTER_N].buffer[7], "   88      `888   ");

            // O
            strcpy(letters_vector[LETTER_O].buffer[0], "    ,ad8888ba,    ");
            strcpy(letters_vector[LETTER_O].buffer[1], "   d8\"'    `\"8b   ");
            strcpy(letters_vector[LETTER_O].buffer[2], "  d8'        `8b  ");
            strcpy(letters_vector[LETTER_O].buffer[3], "  88          88  ");
            strcpy(letters_vector[LETTER_O].buffer[4], "  88          88  ");
            strcpy(letters_vector[LETTER_O].buffer[5], "  Y8,        ,8P  ");
            strcpy(letters_vector[LETTER_O].buffer[6], "   Y8a.    .a8P   ");
            strcpy(letters_vector[LETTER_O].buffer[7], "    `\"Y8888Y\"'    ");

            // P
            strcpy(letters_vector[LETTER_P].buffer[0], "   888888888ba    ");
            strcpy(letters_vector[LETTER_P].buffer[1], "   88       \"8b   ");
            strcpy(letters_vector[LETTER_P].buffer[2], "   88       ,8P   ");
            strcpy(letters_vector[LETTER_P].buffer[3], "   88aaaaaaa8P'   ");
            strcpy(letters_vector[LETTER_P].buffer[4], "   88\"\"\"\"\"\"\"'     ");
            strcpy(letters_vector[LETTER_P].buffer[5], "   88             ");
            strcpy(letters_vector[LETTER_P].buffer[6], "   88             ");
            strcpy(letters_vector[LETTER_P].buffer[7], "   88             ");

            // Q
            strcpy(letters_vector[LETTER_Q].buffer[0], "    ,ad8888ba,    ");
            strcpy(letters_vector[LETTER_Q].buffer[1], "   d8\"'    `\"8b   ");
            strcpy(letters_vector[LETTER_Q].buffer[2], "  d8'        `8b  ");
            strcpy(letters_vector[LETTER_Q].buffer[3], "  88          88  ");
            strcpy(letters_vector[LETTER_Q].buffer[4], "  88          88  ");
            strcpy(letters_vector[LETTER_Q].buffer[5], "  Y8,    \"88,,8P  ");
            strcpy(letters_vector[LETTER_Q].buffer[6], "   Y8a.    Y88P   ");
            strcpy(letters_vector[LETTER_Q].buffer[7], "    `\"Y8888Y\"Y8a  ");

            // R
            strcpy(letters_vector[LETTER_R].buffer[0], "   888888888ba    ");
            strcpy(letters_vector[LETTER_R].buffer[1], "   88       \"8b   ");
            strcpy(letters_vector[LETTER_R].buffer[2], "   88       ,8P   ");
            strcpy(letters_vector[LETTER_R].buffer[3], "   88aaaaaaa8P'   ");
            strcpy(letters_vector[LETTER_R].buffer[4], "   88\"\"\"\"\"88'     ");
            strcpy(letters_vector[LETTER_R].buffer[5], "   88     `8b     ");
            strcpy(letters_vector[LETTER_R].buffer[6], "   88      `8b    ");
            strcpy(letters_vector[LETTER_R].buffer[7], "   88       `8b   ");

            // S
            strcpy(letters_vector[LETTER_S].buffer[0], "    ad888888ba    ");
            strcpy(letters_vector[LETTER_S].buffer[1], "   d8\"      \"8b   ");
            strcpy(letters_vector[LETTER_S].buffer[2], "   Y8,            ");
            strcpy(letters_vector[LETTER_S].buffer[3], "   `Y8aaaaaa,     ");
            strcpy(letters_vector[LETTER_S].buffer[4], "     `\"\"\"\"\"\"8b,   ");
            strcpy(letters_vector[LETTER_S].buffer[5], "            `8b   ");
            strcpy(letters_vector[LETTER_S].buffer[6], "   Y8a      a8P   ");
            strcpy(letters_vector[LETTER_S].buffer[7], "    \"Y888888P\"    ");

            // T
            strcpy(letters_vector[LETTER_T].buffer[0], "   888888888888   ");
            strcpy(letters_vector[LETTER_T].buffer[1], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[2], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[3], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[4], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[5], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[6], "        88        ");
            strcpy(letters_vector[LETTER_T].buffer[7], "        88        ");

            // U
            strcpy(letters_vector[LETTER_U].buffer[0], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[1], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[2], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[3], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[4], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[5], "   88        88   ");
            strcpy(letters_vector[LETTER_U].buffer[6], "   Y8a.    .a8P   ");
            strcpy(letters_vector[LETTER_U].buffer[7], "    `\"Y8888Y\"'    ");

            // V
            strcpy(letters_vector[LETTER_V].buffer[0], " 8b            d8 ");
            strcpy(letters_vector[LETTER_V].buffer[1], " `8b          d8' ");
            strcpy(letters_vector[LETTER_V].buffer[2], "  `8b        d8'  ");
            strcpy(letters_vector[LETTER_V].buffer[3], "   `8b      d8'   ");
            strcpy(letters_vector[LETTER_V].buffer[4], "    `8b    d8'    ");
            strcpy(letters_vector[LETTER_V].buffer[5], "     `8b  d8'     ");
            strcpy(letters_vector[LETTER_V].buffer[6], "      `8888'      ");
            strcpy(letters_vector[LETTER_V].buffer[7], "       `88'       ");

            // W
            strcpy(letters_vector[LETTER_W].buffer[0], "I8,            ,8I");
            strcpy(letters_vector[LETTER_W].buffer[1], "`8b            d8'");
            strcpy(letters_vector[LETTER_W].buffer[2], " \"8,          ,8\" ");
            strcpy(letters_vector[LETTER_W].buffer[3], "  Y8    db    8P  ");
            strcpy(letters_vector[LETTER_W].buffer[4], "  `8b   aa   d8'  ");
            strcpy(letters_vector[LETTER_W].buffer[5], "   `8a  88  a8'   ");
            strcpy(letters_vector[LETTER_W].buffer[6], "    `8a'  'a8'    ");
            strcpy(letters_vector[LETTER_W].buffer[7], "     `8'  '8'     ");

            // X
            strcpy(letters_vector[LETTER_X].buffer[0], "   8b        d8   ");
            strcpy(letters_vector[LETTER_X].buffer[1], "    Y8,    ,8P    ");
            strcpy(letters_vector[LETTER_X].buffer[2], "     `8b  d8'     ");
            strcpy(letters_vector[LETTER_X].buffer[3], "       Y88P       ");
            strcpy(letters_vector[LETTER_X].buffer[4], "       d88b       ");
            strcpy(letters_vector[LETTER_X].buffer[5], "     ,8P  Y8,     ");
            strcpy(letters_vector[LETTER_X].buffer[6], "    d8'    `8b    ");
            strcpy(letters_vector[LETTER_X].buffer[7], "   8P        Y8   ");

            // Y
            strcpy(letters_vector[LETTER_Y].buffer[0], "   8b        d8   ");
            strcpy(letters_vector[LETTER_Y].buffer[1], "    Y8,    ,8P    ");
            strcpy(letters_vector[LETTER_Y].buffer[2], "     Y8,  ,8P     ");
            strcpy(letters_vector[LETTER_Y].buffer[3], "      \"8aa 8\"     ");
            strcpy(letters_vector[LETTER_Y].buffer[4], "       `88'       ");
            strcpy(letters_vector[LETTER_Y].buffer[5], "        88        ");
            strcpy(letters_vector[LETTER_Y].buffer[6], "        88        ");
            strcpy(letters_vector[LETTER_Y].buffer[7], "        88        ");

            // Z
            strcpy(letters_vector[LETTER_Z].buffer[0],"   888888888888   ");
            strcpy(letters_vector[LETTER_Z].buffer[1],"            ,88   ");
            strcpy(letters_vector[LETTER_Z].buffer[2],"          ,88\"    ");
            strcpy(letters_vector[LETTER_Z].buffer[3],"        ,88\"      ");
            strcpy(letters_vector[LETTER_Z].buffer[4],"      ,88\"        ");
            strcpy(letters_vector[LETTER_Z].buffer[5],"    ,88\"          ");
            strcpy(letters_vector[LETTER_Z].buffer[6],"   88\"            ");
            strcpy(letters_vector[LETTER_Z].buffer[7],"   888888888888   ");

    //         break;
    //     case DOOM:

    //         break;
    //     case VARSITY:

    //         break;
    // }
}



// for i in range (26) :
//     for j in range(8):
//         read from ASII art file
        
//         currentLine = currentLine + 1
    
//     currentLine = currentLine + 1
