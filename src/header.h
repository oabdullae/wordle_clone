#ifndef HEADER_H
#define HEADER_H

#define MENU_WIDTH  102
#define MENU_HEIGHT 55

typedef enum {
    ROW,
    COL
} Coordinates;

typedef enum {
    NEW_GAME,
    CONTINUE,
    SETTINGS,
    QUIT
} menu_options;

#endif