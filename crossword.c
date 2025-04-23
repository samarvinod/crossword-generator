#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "cwfunc.h"

#define BOARD_SIZE 15
#define MAX_WORDS 20
#define MAX_LENGTH 15

int main(int argc, char *argv[]) {
    char board[BOARD_SIZE][BOARD_SIZE];
    char puzzle[BOARD_SIZE][BOARD_SIZE];
    struct Word words[MAX_WORDS];
    int wordCount = 0;
    FILE *input = stdin;
    FILE *output = stdout;  // Default output is stdout
    
    srand(time(NULL));
    
    if (argc > 3) {
        printf("Error: Too many arguments\n");
        return 1;
    }
    
    // Open input file if specified
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            printf("Error: Cannot open input file\n");
            return 1;
        }
    }
    
    // Open output file if specified
    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (!output) {
            printf("Error: Cannot open output file\n");
            return 1;
        }
    }
    
    initBoard(board, '.');
    initBoard(puzzle, '#');
    
    fprintf(output,"\nAnagram Crossword Puzzle Generator\n");
    fprintf(output,"----------------------------------\n\n");
    
    // Collect words based on whether we have a file or stdin
    if (argc == 1) {
        collectWordsFromInput(words, &wordCount);
    } else {
        collectWordsFromFile(input, words, &wordCount);
    }
    
    if (input != stdin) {
        fclose(input);
    }
    
    // Sort words by length in descending order
    sortWordsByLength(words, wordCount);
    
    if (wordCount > 0) {
        placeFirstWord(board, words, 0);
        
        for (int i = 1; i < wordCount; i++) {
            if (!tryPlaceWord(board, words, i)) {
                fprintf(output, "Could not place word: %s\n", words[i].text);
            }
        }
    }
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            puzzle[i][j] = (board[i][j] == '.') ? '#' : ' ';
        }
    }
    
    fprintf(output, "\nSolution:\n");
    printBoard(output, board);
    
    fprintf(output, "Crossword Puzzle:\n");
    printBoard(output, puzzle);
    
    fprintf(output, "Clues:\n");
    for (int i = 0; i < wordCount; i++) {
        if (words[i].placed) {
            char anagram[MAX_LENGTH + 1];
            scrambleWord(words[i].text, anagram);
            fprintf(output, "%2d,%2d %s   %s\n", 
                   words[i].row, 
                   words[i].col,
                   words[i].direction ? "Down  " : "Across",
                   anagram);
        }
    }
    fprintf(output, "\n");
    
    if (output != stdout) {
        fclose(output);
    }
    
    return 0;
}
