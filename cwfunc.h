#include <stdio.h>

#define BOARD_SIZE 15
#define MAX_WORDS 20
#define MAX_LENGTH 15

struct Word {
    char text[MAX_LENGTH + 1];  // Word text (max length 15)
    int row;                    // Row position of the word
    int col;                    // Column position of the word
    int direction;              // 0 for horizontal, 1 for vertical
    int placed;                 // 1 if the word is placed, 0 otherwise
};

void initBoard(char board[BOARD_SIZE][BOARD_SIZE], char fillChar);
void printBoard(FILE *output, char board[BOARD_SIZE][BOARD_SIZE]);
void toUpper(char *str);
int isValidWord(char *str);
void scrambleWord(char *input, char *output);
void sortWordsByLength(struct Word words[], int wordCount);
int collectWordsFromFile(FILE *input, struct Word words[], int *wordCount);
int collectWordsFromInput(struct Word words[], int *wordCount);
void placeFirstWord(char board[BOARD_SIZE][BOARD_SIZE], struct Word words[], int index);
int tryPlaceWord(char board[BOARD_SIZE][BOARD_SIZE], struct Word words[], int currentIndex);
