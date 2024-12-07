#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    clock_t startTime = clock();

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %d\n", totalResult);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    usTime = clock() - startTime;

    printf("Part 2 Solution: %d\n", totalResult);
    printf("Part 2 Time: %ld us\n", usTime);

    free(wordSearch);

    return 0;
}