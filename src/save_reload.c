#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "header.h"

void save_session(Game_Session *gamesession) {
    FILE *file = fopen("saved_game.bin", "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fwrite(gamesession, sizeof(Game_Session), 1, file);
    fclose(file);
}

int load_session(Game_Session *session) {
    FILE *file = fopen("saved_game.bin", "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return 0; // fail
    }

    if (fread(session, sizeof(Game_Session), 1, file) != 1) {
        perror("Failed to read session");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1; // success
}
