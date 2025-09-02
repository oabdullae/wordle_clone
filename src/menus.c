#include <ncurses.h>
#include "header.h"

void spiral_clearing_animation(int menu_start_row, int menu_start_col);
void change_cursor(Game_Session *game_session, int action);

int main_menu(int window_size[2]) {
    int menu_start_col = (window_size[COL] - MENU_WIDTH)/2, menu_start_row = (window_size[ROW] - MENU_HEIGHT)/2; // they start with borders included btw
    int current_row = menu_start_row + 1;
    int underlines_positions[4]; // to store the lines that show what option is selected

    mvprintw(menu_start_row-1, menu_start_col, "Esc to Pause");
    mvprintw(menu_start_row-1, menu_start_col + MENU_WIDTH - 5, "00:00"); // for time to implement later

    // drawing main menu border
    attron(A_STANDOUT);
    // horizontal border-sides
    for (int i = 0; i < MENU_WIDTH; ++i) {
        mvaddch(menu_start_row, menu_start_col + i, ' ');
        mvaddch(menu_start_row + MENU_HEIGHT-1, menu_start_col + i, ' ');
    }

    // vertical border-sides
    for (int i = menu_start_row; i < MENU_HEIGHT + menu_start_row; ++i) {
        mvprintw(i, menu_start_col, "  "); // left border-column
        mvprintw(i, menu_start_col + MENU_WIDTH-2, "  "); // right border-column
    }
    attroff(A_STANDOUT);


    current_row++;
    mvprintw(current_row++, (window_size[COL] - 90)/2, " I8,            ,8I   ,ad8888ba,   888888888ba  888888888ba,   88           888888888888 ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, " `8b            d8'  d8\"'    `\"8b  88       \"8b 88       `\"8b  88           88           ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "  \"8,          ,8\"  d8'        `8b 88       ,8P 88         `8b 88           88           ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "   Y8    db    8P   88          88 88aaaaaaa8P' 88          88 88           88aaaaaa     ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "   `8b   aa   d8'   88          88 88\"\"\"\"\"88'   88          88 88           88\"\"\"\"\"\"     ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "    `8a  88  a8'    Y8,        ,8P 88     `8b   88          8P 88           88           ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "     `8a'  'a8'      Y8a.    .a8P  88      `8b  88       .a8P  88           88           ");
    mvprintw(current_row++, (window_size[COL] - 90)/2, "      `8'  '8'        `\"Y8888Y\"'   88       `8b 888888888Y\"'   888888888888 888888888888 ");

    current_row++; // skip line
    mvprintw(current_row++, (window_size[COL] - 70)/2, "  ,ad88888ba,  88             ,ad8888ba,   888b      88 888888888888");
    mvprintw(current_row++, (window_size[COL] - 70)/2, " d8\"'     `\"8b 88            d8\"'    `\"8b  8888b     88 88          ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, "d8'            88           d8'        `8b 88 `8b    88 88          ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, "88             88           88          88 88  `8b   88 88aaaaaa    ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, "88             88           88          88 88   `8b  88 88\"\"\"\"\"\"    ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, "Y8,            88           Y8,        ,8P 88    `8b 88 88          ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, " Y8a.     .a8P 88            Y8a.    .a8P  88     `8888 88          ");
    mvprintw(current_row++, (window_size[COL] - 70)/2, "  `\"Y88888Y\"'  888888888888   `\"Y8888Y\"'   88      `888 888888888888");

    current_row++;
    mvprintw(current_row++, (window_size[COL] - 42)/2, "developped by Anas, Haytam, and Oabdullae");

    current_row++;
    mvprintw(current_row++, (window_size[COL] - 26)/2, "    __      __  _       __");
    mvprintw(current_row++, (window_size[COL] - 26)/2, "|\\||_ | |  /__ |_| |V| |_ ");
    mvprintw(current_row++, (window_size[COL] - 26)/2, "| ||__|^|  \\_| | | | | |__");

    current_row++;
    attron(A_STANDOUT | A_BLINK); // initially NEW GAME is selected, so we hightlight it and make it blink 
    mvprintw(current_row++, (window_size[COL] - 32)/2, "===============================");
    underlines_positions[NEW_GAME] = current_row - 1; // save the NEW GAME underline's row, we subtract one cuz it got incremented in the previous line, and we wanna save the previous line
    attroff(A_STANDOUT | A_BLINK);
    current_row++;
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 32)/2," __  _      ___ ___          __");
    mvprintw(current_row++, (window_size[COL] - 32)/2,"/   / \\ |\\|  |   |  |\\| | | |_ ");
    mvprintw(current_row++, (window_size[COL] - 32)/2,"\\__ \\_/ | |  |  _|_ | | |_| |__");
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 32)/2, "===============================");
    underlines_positions[CONTINUE] = current_row - 1;
    current_row++;
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 32)/2," __  __ ___ ___ ___      __  __");
    mvprintw(current_row++, (window_size[COL] - 32)/2,"(_  |_   |   |   |  |\\| /__ (_ ");
    mvprintw(current_row++, (window_size[COL] - 32)/2,"__) |__  |   |  _|_ | | \\_| __)");
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 32)/2, "===============================");
    underlines_positions[SETTINGS] = current_row - 1;
    current_row++;
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 16)/2, " _      ___ ___");
    mvprintw(current_row++, (window_size[COL] - 16)/2, "/ \\ | |  |   | ");
    mvprintw(current_row++, (window_size[COL] - 16)/2, "\\_X |_| _|_  | ");
    
    current_row++;
    mvprintw(current_row++, (window_size[COL] - 32)/2, "===============================");
    underlines_positions[QUIT] = current_row - 1;

    int menu_choice = NEW_GAME;
    
    int pressed_key;
    do {
        pressed_key = getch();
        switch (pressed_key) {
            case KEY_UP:
                // all attributes are deactivated by default, we write the underline regularly of the 
                mvprintw(underlines_positions[menu_choice], (window_size[COL] - 32)/2, "===============================");
                menu_choice -= (menu_choice == 0) ? -3 : 1;
                // mvprintw(0, 5, "%d", menu_choice);
                attron(A_BLINK | A_STANDOUT);
                mvprintw(underlines_positions[menu_choice], (window_size[COL] - 32)/2, "===============================");
                attroff(A_BLINK | A_STANDOUT);
                break;
            case KEY_DOWN:
                mvprintw(underlines_positions[menu_choice], (window_size[COL] - 32)/2, "===============================");
                // menu_choice += (menu_choice == 3) ? -3 : 1;
                menu_choice = (menu_choice + 1) % 4;
                attron(A_BLINK | A_STANDOUT);
                mvprintw(underlines_positions[menu_choice], (window_size[COL] - 32)/2, "===============================");
                attroff(A_BLINK | A_STANDOUT);
                break;
            case '\n':
                // main menu will fade with a beautiful spiral animation by Haytam
                spiral_clearing_animation(menu_start_row, menu_start_col);
                return menu_choice;
            // default: just ignore it and loop back
        }
    } while (1);
}

int escape_menu(Game_Session* game_session) {
    change_cursor(game_session, DELETE_CURSOR);

    int start_row = game_session->menu_start_row + (CELL_HEIGHT + 1) * 2;
    int start_col = game_session->menu_start_col + 1;


    attron(COLOR_PAIR(GRAY));
    for (int i = 0; i < (CELL_HEIGHT * 2) + 1; ++i) { // i < smth, to be fixed with Anas
        for (int j = 0; j < MENU_WIDTH-4; ++j) {
            mvaddch(start_row+1 + i, start_col+1 + j, ' '); // exact coordinates to be fixed with Anas
        }
    }
    
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "888888888ba         db        88        88  ad888888ba  888888888888");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88       \"8b       d88b       88        88 d8\"      \"8b 88          ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88       ,8P      d8'`8b      88        88 Y8,          88          ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88aaaaaaa8P'     d8'  `8b     88        88 `Y8aaaaaa,   88aaaaaa    ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88\"\"\"\"\"\"\"'      d8YaaaaY8b    88        88   `\"\"\"\"\"\"8b, 88\"\"\"\"\"\"    ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88             d8\"\"\"\"\"\"\"\"8b   88        88          `8b 88          ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88            d8'        `8b  Y8a.    .a8P Y8a      a8P 88          ");
    mvprintw(++start_row, start_col + (MENU_WIDTH - 68) / 2, "88           d8'          `8b  `\"Y8888Y\"'   \"Y888888P\"  888888888888");

    ++start_row; // skip a row

    int common_start_row = start_row;
    #define COMMON_HEIGHT 8 // all options have a height of 8 rows
    
    int save_quit_col = start_col+2 + CELL_WIDTH/2;
    #define SAVE_QUIT_WIDTH 19
    mvprintw(++start_row, save_quit_col, "   __  _       __  ");
    mvprintw(++start_row, save_quit_col, "  (_  |_| \\ / |_   ");
    mvprintw(++start_row, save_quit_col, "  __) | |  V  |__  ");
    mvprintw(++start_row, save_quit_col, "         &         ");
    mvprintw(++start_row, save_quit_col, "   _      ___ ___  ");
    mvprintw(++start_row, save_quit_col, "  / \\ | |  |   |   ");
    mvprintw(++start_row, save_quit_col, "  \\_X |_| _|_  |   ");
    mvprintw(++start_row, save_quit_col, "                   ");
    start_row = common_start_row;
    
    attron(COLOR_PAIR(NO_COLOR)); // selected by default when the menu shows up
    int resume_game_col = start_col + (CELL_WIDTH + 2) * 2;
    #define RESUME_GAME_WIDTH 20 
    mvprintw(++start_row, resume_game_col, "  _  __ __       __ ");
    mvprintw(++start_row, resume_game_col, " |_)|_ (_ | ||V||_  ");
    mvprintw(++start_row, resume_game_col, " | \\|____)|_|| ||__ ");
    mvprintw(++start_row, resume_game_col, "                    ");
    mvprintw(++start_row, resume_game_col, "   __  _       __   ");
    mvprintw(++start_row, resume_game_col, "  /__ |_| |V| |_    ");
    mvprintw(++start_row, resume_game_col, "  \\_| | | | | |__   ");
    mvprintw(++start_row, resume_game_col, "                    ");
    start_row = common_start_row;
    attron(COLOR_PAIR(GRAY));

    int main_menu_col = start_col + (CELL_WIDTH + 2) * 4 - CELL_WIDTH/2 - 1;
    // #define MAIN_MENU_WIDTH 19 // same as SAVE_QUIT_WIDTH
    mvprintw(++start_row, main_menu_col, "       _  ___      ");
    mvprintw(++start_row, main_menu_col, "  |V| |_|  |  |\\|  ");
    mvprintw(++start_row, main_menu_col, "  | | | | _|_ | |  ");
    mvprintw(++start_row, main_menu_col, "                   ");
    mvprintw(++start_row, main_menu_col, "       __          ");
    mvprintw(++start_row, main_menu_col, "  |V| |_  |\\| | |  ");
    mvprintw(++start_row, main_menu_col, "  | | |__ | | |_|  ");
    mvprintw(++start_row, main_menu_col, "                   ");
    start_row = common_start_row;
    attroff(COLOR_PAIR(GRAY));


    
    int menu_choice = RESUME_GAME; // cuz resume game is the default selection at the start of this menu
    int input;
    do {
        input = getch();
        int c, opt_start_col;
        switch (input) {
            case KEY_LEFT:
            case KEY_RIGHT:
                // deleting old highlight
                attron(COLOR_PAIR(GRAY)); // we remove highlight by reprinting the option in gray
                for (int i = 1; i < COMMON_HEIGHT+1; ++i) {
                    for (int j = 0; j < ((menu_choice == RESUME_GAME) ? RESUME_GAME_WIDTH : SAVE_QUIT_WIDTH); ++j) {
                        switch (menu_choice) {
                            case SAVE_AND_QUIT:
                                opt_start_col = save_quit_col;
                                break;
                            case RESUME_GAME:
                                opt_start_col = resume_game_col;
                                break;
                            case MAIN_MENU:
                                opt_start_col = main_menu_col;
                                break;                            
                        }
                        c = mvinch(common_start_row+i, opt_start_col+j) & A_CHARTEXT;
                        mvaddch(common_start_row+i, opt_start_col+j, c);
                    }
                }
                attroff(COLOR_PAIR(GRAY));

                if (input == KEY_LEFT) {
                    // change menu_choice
                    menu_choice = (menu_choice - 1 + 3) % 3;
                    // menu_choice -= (menu_choice == 0) ? -2 : 1;  // for oabdullae's weak brain :(
                }
                else { // KEY_RIGHT
                    // move highlight to the right
                    menu_choice = (menu_choice + 1) % 3;
                }
                
                // printing new highlight, NO_COLOR is the highlight
                for (int i = 1; i < COMMON_HEIGHT+1; ++i) {
                    for (int j = 0; j < ((menu_choice == RESUME_GAME) ? RESUME_GAME_WIDTH : SAVE_QUIT_WIDTH); ++j) {
                        switch (menu_choice) {
                            case SAVE_AND_QUIT:
                                opt_start_col = save_quit_col;
                                break;
                            case RESUME_GAME:
                                opt_start_col = resume_game_col;
                                break;
                            case MAIN_MENU:
                                opt_start_col = main_menu_col;
                                break;                            
                        }
                        c = mvinch(common_start_row+i, opt_start_col+j) & A_CHARTEXT;
                        mvaddch(common_start_row+i, opt_start_col+j, c);
                    }
                }
                break;
            case ESCAPE_KEY:
                return RESUME_GAME;
                break;
            case '\n':
                return menu_choice;
                break;
            // default: // ignore it and loop back
        }
    } while (1);
    

/*
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
*/
}

