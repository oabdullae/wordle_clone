#include <ncurses.h>
#include <unistd.h>

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

void cell_grid_animation(int top, int left, int bottom, int right,int window_size[2]){
    int speed=10000;
    int menu_start_col = (window_size[COL] - MENU_WIDTH)/2;
    int menu_start_row = (window_size[ROW] - MENU_HEIGHT)/2; // they start with borders included btw
    attron(A_STANDOUT);
    for (int i = top, k=0;k<MENU_WIDTH-2 || i <= bottom ; i++,k++) {
        if(i <= bottom){
                //from top to bottom
                mvprintw(i , menu_start_col + (2 + CELL_WIDTH)*1, "  ");
                mvprintw(i , menu_start_col + (2 + CELL_WIDTH)*2, "  ");
                //from bottom to top
                mvprintw((bottom+2) - i , menu_start_col + (2 + CELL_WIDTH)*3, "  ");
                mvprintw((bottom+2) - i , menu_start_col + (2 + CELL_WIDTH)*4, "  ");
        }
        if(k<MENU_WIDTH-2){
                //from left to right
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*1 , left+k, ' ');
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*2 , left+k , ' ');
                //from right to left
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*4 , right-k , ' ');
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*5 , right-k  , ' ');
                //from med to born
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*3 , left + ((right-left )/2) + (k/2) , ' ');
                mvaddch(menu_start_row + (1 + CELL_HEIGHT)*3 , left + ((right-left )/2 )- (k/2) , ' ');
        }
        refresh();
        usleep(speed);
    }
    attroff(A_STANDOUT);

}
