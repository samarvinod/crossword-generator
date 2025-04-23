# Crossword Generator

This is a Crossword Generator, where the user can provide a list of words to create a crossword puzzle with. They can provide the words through the terminal or even an input file with a list of words that you want the crossword to include. Moreover, the user has an option to provide a file for the program to print the results to. The generator will attempt to place all the words into the puzzle, but will notify them if it cannot place the word. The generator gives the user hints to each of the words. The hints are scrambled versions of the words. 

Note: There is a 15 letter limit for each word given to the program.

Instructions:
1. Run the comman 'make' in the terminal to create an executable file.
2. Run the executable './runcrossword', then provide a word and press 'Enter', and repeat. Type '.' to stop.
3. If you would like to provide an input file, run the executable './runcrossword' followed by an input file. For example: './runcrossword input.txt'.
5. Optionally, you can run './runcrossword input.txt output.txt' to print out the results of the crossword - made with words from input.txt - into output.txt instead of the terminal.
