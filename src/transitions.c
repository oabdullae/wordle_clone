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
