#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    // Parse Input
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("File not found\n");
        return 1;
    }

    int size = 0, colSize = 0, rowSize = 1, initSize = 1000, c; 
    uint8_t* wordSearch = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (size == initSize) {
            initSize *= 2;
            wordSearch = realloc(wordSearch, initSize * sizeof(uint8_t));
        }
        if (c != '\n') {
            wordSearch[size] = c;
            size++;
        } else {
            if (colSize == 0) colSize = size;
            rowSize++;
        }
    }
    fclose(fptr);

    // Part 1 - O(n)
    int occurrences = 0;
    uint8_t (*wordSearch2D)[colSize] = (uint8_t (*)[colSize]) wordSearch;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (wordSearch2D[i][j] == 'X') {
                // Up
                if (i > 2 && wordSearch2D[i - 1][j] == 'M' && wordSearch2D[i - 2][j] == 'A' && wordSearch2D[i - 3][j] == 'S') {
                    occurrences++;
                }
                // Up Right
                if (i > 2 && j < colSize - 3 && wordSearch2D[i - 1][j + 1] == 'M' && wordSearch2D[i - 2][j + 2] == 'A' && wordSearch2D[i - 3][j + 3] == 'S') {
                    occurrences++;
                }
                // Right
                if (j < colSize - 3 && wordSearch2D[i][j + 1] == 'M' && wordSearch2D[i][j + 2] == 'A' && wordSearch2D[i][j + 3] == 'S') {
                    occurrences++;
                }
                // Down Right
                if (i < rowSize - 3 && j < colSize - 3 && wordSearch2D[i + 1][j + 1] == 'M' && wordSearch2D[i + 2][j + 2] == 'A' && wordSearch2D[i + 3][j + 3] == 'S') {
                    occurrences++;
                }
                // Down
                if (i < rowSize - 3 && wordSearch2D[i + 1][j] == 'M' && wordSearch2D[i + 2][j] == 'A' && wordSearch2D[i + 3][j] == 'S') {
                    occurrences++;
                }
                // Down Left
                if (i < rowSize - 3 && j > 2 && wordSearch2D[i + 1][j - 1] == 'M' && wordSearch2D[i + 2][j - 2] == 'A' && wordSearch2D[i + 3][j - 3] == 'S') {
                    occurrences++;
                }
                // Left
                if (j > 2 && wordSearch2D[i][j - 1] == 'M' && wordSearch2D[i][j - 2] == 'A' && wordSearch2D[i][j - 3] == 'S') {
                    occurrences++;
                }
                // Up Left
                if (i > 2 && j > 2 && wordSearch2D[i - 1][j - 1] == 'M' && wordSearch2D[i - 2][j - 2] == 'A' && wordSearch2D[i - 3][j - 3] == 'S') {
                    occurrences++;
                }
            }
        }
    }

    printf("Part 1 Solution: %d\n", occurrences);

    // Part 2 - O(n)
    occurrences = 0;
    for (int i = 1; i < rowSize - 1; i++) {
        for (int j = 1; j < colSize - 1; j++) {
            if (wordSearch2D[i][j] == 'A') {
                /* M . S
                   . A .
                   M . S */
                if (wordSearch2D[i - 1][j - 1] == 'M' && wordSearch2D[i - 1][j + 1] == 'S' && wordSearch2D[i + 1][j - 1] == 'M' && wordSearch2D[i + 1][j + 1] == 'S') {
                    occurrences++;
                }
                /* M . M
                   . A .
                   S . S */
                if (wordSearch2D[i - 1][j - 1] == 'M' && wordSearch2D[i - 1][j + 1] == 'M' && wordSearch2D[i + 1][j - 1] == 'S' && wordSearch2D[i + 1][j + 1] == 'S') {
                    occurrences++;
                }
                /* S . M
                   . A .
                   S . M */
                if (wordSearch2D[i - 1][j - 1] == 'S' && wordSearch2D[i - 1][j + 1] == 'M' && wordSearch2D[i + 1][j - 1] == 'S' && wordSearch2D[i + 1][j + 1] == 'M') {
                    occurrences++;
                }
                /* S . S
                   . A .
                   M . M */
                if (wordSearch2D[i - 1][j - 1] == 'S' && wordSearch2D[i - 1][j + 1] == 'S' && wordSearch2D[i + 1][j - 1] == 'M' && wordSearch2D[i + 1][j + 1] == 'M') {
                    occurrences++;
                }               
            }
        }
    }
    
    printf("Part 2 Solution: %d\n", occurrences);

    free(wordSearch);

    return 0;
}