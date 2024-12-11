#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint8_t checkLoop(int colSize, int rowSize, uint8_t (*map2D)[colSize], int row, int col);

const int8_t directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int main (int argc, char *argv[]) {
    // Parse Input
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        puts("File not found");
        return 1;
    }

    int size = 0, colSize = 0, rowSize = 1, initSize = 1000, c, initRow, initCol;
    uint8_t* map = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (size == initSize) {
            initSize *= 2;
            map = realloc(map, initSize * sizeof(uint8_t));
        }
        if (c == '^') {
            initRow = rowSize - 1;
            if (colSize) {
                initCol = size % colSize;
            } else {
                initCol = size;
            } 
        }
        if (c != '\n') {
            map[size] = c;
            size++;
        } else {
            if (colSize == 0) colSize = size;
            rowSize++;
        }
    }
    fclose(fptr);    

    // Part 1 - O(n)
    clock_t startTime = clock();

    int distinctPositions = 0, row = initRow, col = initCol, directionsIndex = 0;
    uint8_t (*map2D)[colSize] = (uint8_t (*)[colSize]) map;
    while (1) {
        if (map2D[row][col] != 'X') {
            distinctPositions++;
            map2D[row][col] = 'X';
        }
        int r = row + directions[directionsIndex][0];
        int c = col + directions[directionsIndex][1];
        if (r < 0 || r >= rowSize || c < 0 || c >= colSize) {
            break;
        }
        if (map2D[r][c] == '#') {
            directionsIndex = (directionsIndex + 1) % 4;
        } else {
            row = r;
            col = c;
        }
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %d\n", distinctPositions);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n^2)
    startTime = clock();

    int obstructions = 0;
    row = initRow, col = initCol, directionsIndex = 0;
    while (1) {
        int r = row + directions[directionsIndex][0];
        int c = col + directions[directionsIndex][1];
        if (r < 0 || r >= rowSize || c < 0 || c >= colSize) break;
        if (map2D[r][c] == '#') {
            directionsIndex = (directionsIndex + 1) % 4;
        } else {
            if ((r != initRow || c != initCol) && map2D[r][c] != 'O') {
                map2D[r][c] = '#';
                if (checkLoop(colSize, rowSize, map2D, initRow, initCol)) obstructions++;
                map2D[r][c] = 'O';
            }
            row = r;
            col = c;
        }
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %d\n", obstructions);
    printf("Part 2 Time: %ld us\n", usTime);

    free(map);

    return 0;
}

uint8_t checkLoop(int colSize, int rowSize, uint8_t (*map2D)[colSize], int row, int col) {
    int directionsIndex = 0, r, c;
    uint8_t visitedDirections[rowSize][colSize][4];
    memset(visitedDirections, 0, rowSize * colSize * 4 * sizeof(uint8_t));
    while (1) {
        visitedDirections[row][col][directionsIndex] = 1;
        r = row + directions[directionsIndex][0];
        c = col + directions[directionsIndex][1];
        if (r < 0 || r >= rowSize || c < 0 || c >= colSize) return 0;
        if (map2D[r][c] == '#') {
            directionsIndex = (directionsIndex + 1) % 4;
        } else {
            if (visitedDirections[r][c][directionsIndex]) return 1;
            row = r;
            col = c;
        }
    }
}