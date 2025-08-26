#include <ncurses.h>
#include "header.h"

int main_menu(int window_size[2]) {
    int current_row = 1;
    int underlines_positions[4]; // to store the lines that show what option is selected

    // drawing main menu border
    attron(A_STANDOUT);
    // horizontal border-sides
    mvprintw(0, (window_size[COL] - MENU_WIDTH)/2, "                                                                                              ");
    mvprintw(MENU_HEIGHT-1, (window_size[COL] - MENU_WIDTH)/2, "                                                                                              ");

    // vertical border-sides
    for (int i = 1; i < MENU_HEIGHT; ++i) {
        mvprintw(i, (window_size[COL] - MENU_WIDTH)/2, "  "); // left border-column
        mvprintw(i, (window_size[COL] - MENU_WIDTH)/2 + MENU_WIDTH-2, "  "); // right border-column
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
                // fading_animation(); // main menu will fade with a beautiful animation by Haytam
                return menu_choice;
            // default: just ignore it and loop back
        }
    } while (pressed_key != KEY_ENTER);
}
