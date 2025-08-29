#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
// #include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <ncurses.h>
#include "header.h"

#define WORDS_NUMBER 2315
#define LINE_NUM_B 141
#define LINE_NUM_C 314
#define LINE_NUM_D 512
#define LINE_NUM_E 623
#define LINE_NUM_F 695
#define LINE_NUM_G 831
#define LINE_NUM_H 946
#define LINE_NUM_I 1015
#define LINE_NUM_J 1049
#define LINE_NUM_K 1069
#define LINE_NUM_L 1089
#define LINE_NUM_M 1177
#define LINE_NUM_N 1284
#define LINE_NUM_O 1321
#define LINE_NUM_P 1362
#define LINE_NUM_Q 1504
#define LINE_NUM_R 1527
#define LINE_NUM_S 1632
#define LINE_NUM_T 1998
#define LINE_NUM_U 2147
#define LINE_NUM_V 2180
#define LINE_NUM_W 2223
#define LINE_NUM_Y 2306
#define LINE_NUM_Z 2312

int pick_random_word(char *buffer) {
    int fd = open("wordle_answers.txt", O_RDONLY);

    if (fd == -1) {
        curs_set(1);
        endwin();
        perror("open");
        fflush(stderr);
        exit(1);
    }

    if (lseek(fd, (rand() % WORDS_NUMBER) * 6, SEEK_SET) == -1) {
        close(fd);
        curs_set(1);
        endwin();
        perror("lseek");
        fflush(stderr);
        exit(1);
    }

    int bytesRead = read(fd, buffer, WORD_LENGTH);
    if (bytesRead != 5) {
        close(fd);
        curs_set(1);
        endwin();
        perror("read");
        fflush(stderr);
        exit(1);
    }

    buffer[5] = '\0';
    close(fd);
    return 0;
}

bool is_word_english(char* word){
    int start_line = 0, end_line = WORDS_NUMBER-1;

    for (int i = 0; i < 5; i++)
        word[i] = tolower((unsigned char)word[i]);

    if(word[0] < 'a' || word[0] > 'z'){
        curs_set(1);
        endwin();
        exit(1);
    }
        

    switch (word[0]){
        case 'b': start_line = LINE_NUM_B; end_line = LINE_NUM_C-1; break;
        case 'c': start_line = LINE_NUM_C; end_line = LINE_NUM_D-1; break;
        case 'd': start_line = LINE_NUM_D; end_line = LINE_NUM_E-1; break;
        case 'e': start_line = LINE_NUM_E; end_line = LINE_NUM_F-1; break;
        case 'f': start_line = LINE_NUM_F; end_line = LINE_NUM_G-1; break;
        case 'g': start_line = LINE_NUM_G; end_line = LINE_NUM_H-1; break;
        case 'h': start_line = LINE_NUM_H; end_line = LINE_NUM_I-1; break;
        case 'i': start_line = LINE_NUM_I; end_line = LINE_NUM_J-1; break;
        case 'j': start_line = LINE_NUM_J; end_line = LINE_NUM_K-1; break;
        case 'k': start_line = LINE_NUM_K; end_line = LINE_NUM_L-1; break;
        case 'l': start_line = LINE_NUM_L; end_line = LINE_NUM_M-1; break;
        case 'm': start_line = LINE_NUM_M; end_line = LINE_NUM_N-1; break;
        case 'n': start_line = LINE_NUM_N; end_line = LINE_NUM_O-1; break;
        case 'o': start_line = LINE_NUM_O; end_line = LINE_NUM_P-1; break;
        case 'p': start_line = LINE_NUM_P; end_line = LINE_NUM_Q-1; break;
        case 'q': start_line = LINE_NUM_Q; end_line = LINE_NUM_R-1; break;
        case 'r': start_line = LINE_NUM_R; end_line = LINE_NUM_S-1; break;
        case 's': start_line = LINE_NUM_S; end_line = LINE_NUM_T-1; break;
        case 't': start_line = LINE_NUM_T; end_line = LINE_NUM_U-1; break;
        case 'u': start_line = LINE_NUM_U; end_line = LINE_NUM_V-1; break;
        case 'v': start_line = LINE_NUM_V; end_line = LINE_NUM_W-1; break;
        case 'w': start_line = LINE_NUM_W; end_line = LINE_NUM_Y-1; break;
        case 'x': return false;
        case 'y': start_line = LINE_NUM_Y; end_line = LINE_NUM_Z-1; break;
        case 'z': start_line = LINE_NUM_Z; end_line = WORDS_NUMBER-1; break;
        default: start_line = 0; end_line = LINE_NUM_B-1;
    }

    char buffer[6]; 
    int low = start_line;
    int high = end_line; 

    int fd = open("wordle_answers.txt", O_RDONLY);
    if (fd == -1) {
        curs_set(1);
        endwin();
        perror("open");
        fflush(stderr);
        exit(1);
    }

    while (low <= high) {
        int mid = (low + high) / 2;

        if (lseek(fd, mid * (WORD_LENGTH+1), SEEK_SET) == -1) {
            close(fd);
            curs_set(1);
            endwin();
            perror("lseek");
            fflush(stderr);
            exit(1);
        }

        int bytesRead = read(fd, buffer, WORD_LENGTH);
        if (bytesRead != WORD_LENGTH) {
            close(fd);
            curs_set(1);
            endwin();
            perror("read");
            fflush(stderr);
            exit(1);
        }
        buffer[WORD_LENGTH] = '\0'; 

        int cmp = strcmp(buffer, word);

        if (cmp == 0) {
            close(fd);
            return true; 
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    close(fd);
    return false; 
}

#if 0
int main (){
    srand(time(NULL) * getpid());
    char buffer[6], word[6];

    int success = random_word(buffer);
    if (success != -1) {
        printf("Random word: %s\n", buffer);
        do{
            printf("enter ur word : ");
            scanf("%s",word);
        }while(is_valid(word) != true);
    }
    return 0;
}
#endif