# Crossword Generator

This is a Crossword Generator program that creates crossword puzzles based on a list of words provided by the user. Users can input words manually through the terminal or supply an input file containing the words they’d like to include in the puzzle. The program will attempt to place all the words into the crossword grid. If a word cannot be placed, the user will be notified.

Each word in the puzzle comes with a hint, which is a scrambled version of the word itself.

Note: Each word must be no longer than 15 letters.

## Features
✅ Accepts words from terminal input or a file.
✅ Optional output file to save the generated puzzle.
✅ Provides scrambled hints for each word.
✅ Notifies the user if a word cannot be placed.

## Instructions
### 1. Build the program:
```bash
make
```

### 2. Run the program interactively:
```bash
./runcrossword
```
- Enter a word, press Enter, and repeat as needed.
- Type a single period . to finish entering words.

### 3. Run the program with an input file:
```bash
./runcrossword input.txt
```
- This uses the words in input.txt to generate the crossword.

### 4. Run the program with both input and output files:
```bash
./runcrossword input.txt output.txt
```
- This uses the words in input.txt and saves the crossword to output.txt instead of displaying it in the terminal.
