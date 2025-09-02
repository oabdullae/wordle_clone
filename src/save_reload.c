#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "header.h"

void save_session(Game_Session *gamesession) {
    getch();
    FILE *file = fopen("saved_game.bin", "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fwrite(gamesession, sizeof(Game_Session), 1, file);
    fclose(file);
}

Game_Session *load_session() {
    FILE *file = fopen("saved_game.bin", "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return NULL;
    }

    Game_Session *session = malloc(sizeof(Game_Session));
    if (!session) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fread(session, sizeof(Game_Session), 1, file);
    fclose(file);

    return session;
}