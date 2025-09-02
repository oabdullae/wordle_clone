#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include "header.h"

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


int wanna_quit(Game_Session* game_session) {
    scr_dump("screen_save.scr");

    int start_y = 24, end_y = 41;
    int start_x = 69, end_x = 167;
    int width   = end_x - start_x;

    const char *top_ascii[] = {
        " __    _  _  __ __       _     _  _ ______ _    ",
        "/  |_|/ \\/ \\(_ |_    | ||_)   / \\|_) |  | / \\|\\|",
        "\\__| |\\_/\\_/__)|__   |_|| \\   \\_/|   | _|_\\_/| |"
    };

    const char *menu_ascii[] = {
        "   _    ______           __            _  __ __       _",
        "  / \\| | |  |        |V||_ |\\|| |     |_)|_ (_ | ||V||_",
        "  \\_X|_|_|_ |        | ||__| ||_|     | \\|____)|_|| ||_"
    };
    
    // Define the horizontal positions for each option in the ASCII art
    typedef struct {
        int start;
        int end;
    } OptionPosition;
    
    OptionPosition quit_positions[] = {
        {2, 14},    // "   _ " on line 0
        {2, 14},    // "  / \\" on line 1  
        {2, 14}     // "  \\_X" on line 2
    };
    
    OptionPosition menu_positions[] = {
        {21, 34},   
        {21, 34},  
        {21, 34}    
    };
    
    OptionPosition resume_positions[] = {
        {38, 55},  
        {38, 55},  
        {38, 55}   
    };

    int top_lines  = 3, menu_lines = 3;
    int top_width  = (int)strlen(top_ascii[0]);
    int menu_width = (int)strlen(menu_ascii[0]);

    int top_row  = start_y + 1;
    int top_col  = start_x + (width - top_width) / 2;
    int menu_row = start_y + 10;
    int menu_col = start_x + (width - menu_width) / 2;

    int selected = 0; // 0=Quit, 1=Menu, 2=Resume


    // Draw background
    attron(COLOR_PAIR(GRAY));
    for (int y = start_y; y < end_y; y++)
        for (int x = start_x; x < end_x; x++)
            mvaddch(y, x, ' ');
    attroff(COLOR_PAIR(GRAY));

    int ch;
    while (1) {
        // Draw title
        attron(COLOR_PAIR(GRAY));
        for (int i = 0; i < top_lines; i++) {
            mvprintw(top_row + i, top_col, "%s", top_ascii[i]);
        }
        attroff(COLOR_PAIR(GRAY));

        // Draw menu ASCII art (base text)
        attron(COLOR_PAIR(GRAY));
        for (int i = 0; i < menu_lines; i++) {
            mvprintw(menu_row + i, menu_col, "%s", menu_ascii[i]);
        }
        attroff(COLOR_PAIR(GRAY));

        // Highlight the selected option
        OptionPosition *positions;
        if (selected == 0) positions = quit_positions;
        else if (selected == 1) positions = menu_positions;
        else positions = resume_positions;
        
        for (int i = 0; i < menu_lines; i++) {
            int start = positions[i].start;
            int end = positions[i].end;
            int len = end - start;
            
            // Move to the position and highlight
            move(menu_row + i, menu_col + start);
            attron(COLOR_PAIR(10) | A_BOLD); // Use the highlight color pair
            for (int j = 0; j < len; j++) {
                addch(menu_ascii[i][start + j]);
            }
            attroff(COLOR_PAIR(10) | A_BOLD);
        }

        refresh();

        ch = getch();
        if (ch == KEY_LEFT) {
            selected = (selected - 1 + 3) % 3;
        } else if (ch == KEY_RIGHT) {
            selected = (selected + 1) % 3;
        } else if (ch == '\n' || ch == KEY_ENTER) {
            if(selected != 1)scr_restore("screen_save.scr");
            if (selected == 0) return 1;    // Quit
            else if (selected == 1) return 2; // Menu
            else return 0;                  // Resume
        }
    }
}


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
