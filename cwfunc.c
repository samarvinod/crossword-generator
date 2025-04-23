#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "cwfunc.h"

#define BOARD_SIZE 15
#define MAX_WORDS 20
#define MAX_LENGTH 15

// Board functions
void initBoard(char board[BOARD_SIZE][BOARD_SIZE], char fillChar) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = fillChar;
        }
    }
}

void printBoard(FILE *output, char board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    fprintf(output, "-----------------\n");
    for (i = 0; i < BOARD_SIZE; i++) {
        fprintf(output, "|");
        for (j = 0; j < BOARD_SIZE; j++) {
            fprintf(output, "%c", board[i][j]);
        }
        fprintf(output, "|\n");
    }
    fprintf(output, "-----------------\n\n");
}

// Word processing functions
void toUpper(char *str) {
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int isValidWord(char *str) {
    int i;
    int len = strlen(str);
    if (len < 2 || len > MAX_LENGTH) return 0;
    
    for (i = 0; i < len; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

void scrambleWord(char *input, char *output) {
    int i;
    int len = strlen(input);
    strcpy(output, input);
    
    for (i = len - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = output[i];
        output[i] = output[j];
        output[j] = temp;
    }
}

// Sort words by length (descending order)
void sortWordsByLength(struct Word words[], int wordCount) {
    int i, j;
    struct Word temp;
    
    for (i = 0; i < wordCount - 1; i++) {
        for (j = i + 1; j < wordCount; j++) {
            if (strlen(words[i].text) < strlen(words[j].text)) {
                // Swap words[i] and words[j]
                temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

// Collect words from a file
int collectWordsFromFile(FILE *input, struct Word words[], int *wordCount) {
    char word[MAX_LENGTH + 1];
    
    while (*wordCount < MAX_WORDS && fscanf(input, "%s", word) == 1) {
        if (strcmp(word, ".") == 0) break; 
        
        if (isValidWord(word)) {
            toUpper(word);
            strcpy(words[*wordCount].text, word);
            words[*wordCount].placed = 0;
            (*wordCount)++;
        } else {
            printf("Invalid word: %s.\n", word);
        }
    }
    return *wordCount;
}

// Collect words from user input
int collectWordsFromInput(struct Word words[], int *wordCount) {
    char word[MAX_LENGTH + 1];
    
    printf("Enter words (type '.' to stop):\n");
    while (*wordCount < MAX_WORDS) {
        printf("> ");
        scanf("%s", word);
        
        if (strcmp(word, ".") == 0) break;
        
        if (isValidWord(word)) {
            toUpper(word);
            strcpy(words[*wordCount].text, word);
            words[*wordCount].placed = 0;
            (*wordCount)++;
        } else {
            printf("Invalid word: %s.\n", word);
        }
    }
    return *wordCount;
}

int findOverlap(char *word1, char *word2, int *pos1, int *pos2) {
    int i, j;
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    
    for (i = 0; i < len1; i++) {
        for (j = 0; j < len2; j++) {
            if (word1[i] == word2[j]) {
                *pos1 = i;
                *pos2 = j;
                return 1;
            }
        }
    }
    return 0;
}

// Placement functions
int canPlaceWordHorizontal(char board[BOARD_SIZE][BOARD_SIZE], char *word, int row, int col) {
    int len = strlen(word);
    int hasIntersection = 0;
    
    // Basic bounds checking
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col + len > BOARD_SIZE)
        return 0;
        
    // Check the row above
    if (row > 0) {
        if (col > 0 && board[row-1][col-1] != '.') return 0;
        if (col + len < BOARD_SIZE && board[row-1][col+len] != '.') return 0;
    }
    
    // Check current row
    if (col > 0 && board[row][col-1] != '.') return 0;
    if (col + len < BOARD_SIZE && board[row][col+len] != '.') return 0;
    
    // Check the row below
    if (row < BOARD_SIZE-1) {
        if (col > 0 && board[row+1][col-1] != '.') return 0;
        if (col + len < BOARD_SIZE && board[row+1][col+len] != '.') return 0;
    }
    
    // Check each position of the word
    for (int i = 0; i < len; i++) {
        if (board[row][col + i] != '.') {
            if (board[row][col + i] != word[i]) return 0;
            hasIntersection = 1;
        } else {
            // Check above and below for adjacent letters
            if (row > 0 && board[row-1][col+i] != '.') return 0;
            if (row < BOARD_SIZE-1 && board[row+1][col+i] != '.') return 0;
        }
    }
    
    return hasIntersection || row == BOARD_SIZE/2;
}

int canPlaceWordVertical(char board[BOARD_SIZE][BOARD_SIZE], char *word, int row, int col) {
    int len = strlen(word);
    int hasIntersection = 0;
    
    // Basic bounds checking
    if (row < 0 || row >= BOARD_SIZE || col < 0 || row + len > BOARD_SIZE)
        return 0;
        
    // Check the column to the left
    if (col > 0) {
        if (row > 0 && board[row-1][col-1] != '.') return 0;
        if (row + len < BOARD_SIZE && board[row+len][col-1] != '.') return 0;
    }
    
    // Check current column
    if (row > 0 && board[row-1][col] != '.') return 0;
    if (row + len < BOARD_SIZE && board[row+len][col] != '.') return 0;
    
    // Check the column to the right
    if (col < BOARD_SIZE-1) {
        if (row > 0 && board[row-1][col+1] != '.') return 0;
        if (row + len < BOARD_SIZE && board[row+len][col+1] != '.') return 0;
    }
    
    // Check each position of the word
    for (int i = 0; i < len; i++) {
        if (board[row + i][col] != '.') {
            if (board[row + i][col] != word[i]) return 0;
            hasIntersection = 1;
        } else {
            // Check left and right for adjacent letters
            if (col > 0 && board[row+i][col-1] != '.') return 0;
            if (col < BOARD_SIZE-1 && board[row+i][col+1] != '.') return 0;
        }
    }
    
    return hasIntersection || row == BOARD_SIZE/2;
}

void placeWordHorizontal(char board[BOARD_SIZE][BOARD_SIZE], char *word, int row, int col) {
    int i;
    int len = strlen(word);
    for (i = 0; i < len; i++) {
        board[row][col + i] = word[i];
    }
}

void placeWordVertical(char board[BOARD_SIZE][BOARD_SIZE], char *word, int row, int col) {
    int i;
    int len = strlen(word);
    for (i = 0; i < len; i++) {
        board[row + i][col] = word[i];
    }
}

// First word placement
void placeFirstWord(char board[BOARD_SIZE][BOARD_SIZE], struct Word words[], int index) {
    int len = strlen(words[index].text);
    int row = BOARD_SIZE / 2;
    int col = (BOARD_SIZE - len) / 2;
    
    placeWordHorizontal(board, words[index].text, row, col);
    words[index].row = row;
    words[index].col = col;
    words[index].direction = 0;
    words[index].placed = 1;
}

int tryPlaceWord(char board[BOARD_SIZE][BOARD_SIZE], struct Word words[], int currentIndex) {
    char *currentWord = words[currentIndex].text;
    int len = strlen(currentWord);
    int bestRow = -1;
    int bestCol = -1;
    int bestDirection = -1;
    int found = 0;
    
    // Try every possible position on the board
    for (int i = 0; i < currentIndex && !found; i++) {
        if (!words[i].placed) continue;
        
        // Try both horizontal and vertical at each position
        for (int letterCurrent = 0; letterCurrent < len && !found; letterCurrent++) {
            char *placedWord = words[i].text;
            int placedLen = strlen(placedWord);
            
            for (int letterPlaced = 0; letterPlaced < placedLen && !found; letterPlaced++) {
                if (currentWord[letterCurrent] == placedWord[letterPlaced]) {
                    // Try horizontal placement
                    if (words[i].direction == 1) {
                        int row = words[i].row + letterPlaced;
                        int col = words[i].col - letterCurrent;
                        
                        if (canPlaceWordHorizontal(board, currentWord, row, col)) {
                            bestRow = row;
                            bestCol = col;
                            bestDirection = 0;
                            found = 1;  // Break out of all loops
                        }
                    }
                    
                    // Try vertical placement
                    if (words[i].direction == 0 && !found) {
                        int row = words[i].row - letterCurrent;
                        int col = words[i].col + letterPlaced;
                        
                        if (canPlaceWordVertical(board, currentWord, row, col)) {
                            bestRow = row;
                            bestCol = col;
                            bestDirection = 1;
                            found = 1;  // Break out of all loops
                        }
                    }
                }
            }
        }
    }
    
    if (bestDirection != -1) {
        if (bestDirection == 0) {
            placeWordHorizontal(board, currentWord, bestRow, bestCol);
        } else {
            placeWordVertical(board, currentWord, bestRow, bestCol);
        }
        words[currentIndex].row = bestRow;
        words[currentIndex].col = bestCol;
        words[currentIndex].direction = bestDirection;
        words[currentIndex].placed = 1;
        return 1;
    }
    
    return 0;
}
