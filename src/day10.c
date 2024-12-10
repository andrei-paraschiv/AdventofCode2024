#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint32_t DFS1(int colSize, int rowSize, uint8_t (*topographicMap2D)[colSize], int row, int col, int goal);
uint32_t DFS2(int colSize, int rowSize, uint8_t (*topographicMap2D)[colSize], int row, int col);

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
    uint8_t* topographicMap = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (size == initSize) {
            initSize *= 2;
            topographicMap = realloc(topographicMap, initSize * sizeof(uint8_t));
        }
        if (c != '\n') {
            topographicMap[size] = c - '0';
            size++;
        } else {
            if (colSize == 0) colSize = size;
            rowSize++;
        }
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    uint8_t (*topographicMap2D)[colSize] = (uint8_t (*)[colSize]) topographicMap;
    uint32_t score = 0, goal = 10;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (topographicMap2D[i][j] == 0) {
                score += DFS1(colSize, rowSize, topographicMap2D, i, j, goal);
                goal++;
            }
        }
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %u\n", score);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    uint32_t rating = 0;
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (topographicMap2D[i][j] == 0) rating += DFS2(colSize, rowSize, topographicMap2D, i, j);
        }
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %u\n", rating);
    printf("Part 2 Time: %ld us\n", usTime);

    free(topographicMap2D);

    return 0;
}

uint32_t DFS1(int colSize, int rowSize, uint8_t (*topographicMap2D)[colSize], int row, int col, int goal) {
    uint32_t score = 0;

    if (topographicMap2D[row][col] == 8) {
        if (row + 1 < rowSize && topographicMap2D[row + 1][col] > 8 && topographicMap2D[row + 1][col] < goal) {
            score++;
            topographicMap2D[row + 1][col] = goal;
        }
        if (col + 1 < colSize && topographicMap2D[row][col + 1] > 8 && topographicMap2D[row][col + 1] < goal) {
            score++;
            topographicMap2D[row][col + 1] = goal;
        }
        if (row - 1 >= 0 && topographicMap2D[row - 1][col] > 8 && topographicMap2D[row - 1][col] < goal) {
            score++;
            topographicMap2D[row - 1][col] = goal;
        }
        if (col - 1 >= 0 && topographicMap2D[row][col - 1] > 8 && topographicMap2D[row][col - 1]  < goal) {
            score++;
            topographicMap2D[row][col - 1]  = goal;
        }
        return score;
    }

    if (row + 1 < rowSize && topographicMap2D[row + 1][col] == topographicMap2D[row][col] + 1) score += DFS1(colSize, rowSize, topographicMap2D, row + 1, col, goal);
    if (col + 1 < colSize && topographicMap2D[row][col + 1] == topographicMap2D[row][col] + 1) score += DFS1(colSize, rowSize, topographicMap2D, row, col + 1, goal);
    if (row - 1 >= 0 && topographicMap2D[row - 1][col] == topographicMap2D[row][col] + 1) score += DFS1(colSize, rowSize, topographicMap2D, row - 1, col, goal);
    if (col - 1 >= 0 && topographicMap2D[row][col - 1] == topographicMap2D[row][col] + 1) score += DFS1(colSize, rowSize, topographicMap2D, row, col - 1, goal);

    return score; 
}

uint32_t DFS2(int colSize, int rowSize, uint8_t (*topographicMap2D)[colSize], int row, int col) {
    uint32_t rating = 0;

    if (topographicMap2D[row][col] == 8) {
        if (row + 1 < rowSize && topographicMap2D[row + 1][col] > 8) rating++;
        if (col + 1 < colSize && topographicMap2D[row][col + 1] > 8) rating++;
        if (row - 1 >= 0 && topographicMap2D[row - 1][col] > 8) rating++;
        if (col - 1 >= 0 && topographicMap2D[row][col - 1] > 8) rating++;
        return rating;
    }

    if (row + 1 < rowSize && topographicMap2D[row + 1][col] == topographicMap2D[row][col] + 1) rating += DFS2(colSize, rowSize, topographicMap2D, row + 1, col);
    if (col + 1 < colSize && topographicMap2D[row][col + 1] == topographicMap2D[row][col] + 1) rating += DFS2(colSize, rowSize, topographicMap2D, row, col + 1);
    if (row - 1 >= 0 && topographicMap2D[row - 1][col] == topographicMap2D[row][col] + 1) rating += DFS2(colSize, rowSize, topographicMap2D, row - 1, col);
    if (col - 1 >= 0 && topographicMap2D[row][col - 1] == topographicMap2D[row][col] + 1) rating += DFS2(colSize, rowSize, topographicMap2D, row, col - 1);

    return rating; 
}