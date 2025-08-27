#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
// #include <time.h>
#include <ctype.h>
#include <stdbool.h>
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
        perror("open");
        return -1;
    }

    if (lseek(fd, (rand() % WORDS_NUMBER) * 6, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return -1;
    }

    int bytesRead = read(fd, buffer, WORD_LENGTH);
    if (bytesRead != 5) {
        perror("read");
        close(fd);
        return -1;
    }

    buffer[5] = '\0';
    close(fd);
    return 0;
}

bool is_word_english(char* word){
    int offset = 0, max_offset = 2315;

    for (int i = 0; i < 5; i++)
        word[i] = tolower((unsigned char)word[i]);

    if(word[0] < 'a' || word[0] > 'z')
        return false;

    switch (word[0]){
        case 'b': offset = LINE_NUM_B; max_offset = LINE_NUM_C-1; break;
        case 'c': offset = LINE_NUM_C; max_offset = LINE_NUM_D-1; break;
        case 'd': offset = LINE_NUM_D; max_offset = LINE_NUM_E-1; break;
        case 'e': offset = LINE_NUM_E; max_offset = LINE_NUM_F-1; break;
        case 'f': offset = LINE_NUM_F; max_offset = LINE_NUM_G-1; break;
        case 'g': offset = LINE_NUM_G; max_offset = LINE_NUM_H-1; break;
        case 'h': offset = LINE_NUM_H; max_offset = LINE_NUM_I-1; break;
        case 'i': offset = LINE_NUM_I; max_offset = LINE_NUM_J-1; break;
        case 'j': offset = LINE_NUM_J; max_offset = LINE_NUM_K-1; break;
        case 'k': offset = LINE_NUM_K; max_offset = LINE_NUM_L-1; break;
        case 'l': offset = LINE_NUM_L; max_offset = LINE_NUM_M-1; break;
        case 'm': offset = LINE_NUM_M; max_offset = LINE_NUM_N-1; break;
        case 'n': offset = LINE_NUM_N; max_offset = LINE_NUM_O-1; break;
        case 'o': offset = LINE_NUM_O; max_offset = LINE_NUM_P-1; break;
        case 'p': offset = LINE_NUM_P; max_offset = LINE_NUM_Q-1; break;
        case 'q': offset = LINE_NUM_Q; max_offset = LINE_NUM_R-1; break;
        case 'r': offset = LINE_NUM_R; max_offset = LINE_NUM_S-1; break;
        case 's': offset = LINE_NUM_S; max_offset = LINE_NUM_T-1; break;
        case 't': offset = LINE_NUM_T; max_offset = LINE_NUM_U-1; break;
        case 'u': offset = LINE_NUM_U; max_offset = LINE_NUM_V-1; break;
        case 'v': offset = LINE_NUM_V; max_offset = LINE_NUM_W-1; break;
        case 'w': offset = LINE_NUM_W; max_offset = LINE_NUM_Y-1; break;
        case 'x': return false;
        case 'y': offset = LINE_NUM_Y; max_offset = LINE_NUM_Z-1; break;
        case 'z': offset = LINE_NUM_Z; max_offset = 2315; break;
        default: offset = 0; max_offset = LINE_NUM_B-1;
    }

    char buffer[6]; 
    int low = offset;
    int high = max_offset; 

    int fd = open("wordle_answers.txt", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open\n");
        return false;
    }

    while (low <= high) {
        int mid = (low + high) / 2;

        if (lseek(fd, mid * 6, SEEK_SET) == -1) {
            perror("lseek");
            close(fd);
            return false;
        }

        int bytesRead = read(fd, buffer, 5);
        if (bytesRead != 5) {
            perror("read");
            close(fd);
            return false;
        }
        buffer[5] = '\0'; 

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