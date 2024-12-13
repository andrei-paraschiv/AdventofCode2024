#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PERIMETER 0
#define AREA 1

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

void regionDFS(uint32_t rowSize, uint32_t colSize, uint8_t (*gardenPlots2D)[colSize], uint8_t (*visited)[colSize], int row, int col, uint32_t* perimeter, uint32_t* area);
uint8_t checkNewSide(uint32_t rowSize, uint32_t colSize, uint8_t (*gardenPlots2D)[colSize], uint8_t (*visitedSides)[colSize][5], int row, int col, uint32_t side);

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

    int c;
    uint32_t size = 0, colSize = 0, rowSize = 1, initSize = 1000; 
    uint8_t* gardenPlots = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (c != '\n') {
            if (size == initSize) {
                initSize *= 2;
                gardenPlots = realloc(gardenPlots, initSize * sizeof(uint8_t));
            }
            gardenPlots[size] = c;
            size++;
        } else {
            if (colSize == 0) colSize = size;
            rowSize++;
        }
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    uint8_t (*gardenPlots2D)[colSize] = (uint8_t (*)[colSize]) gardenPlots, visited[rowSize][colSize];
    uint32_t price = 0, perimeter, area;
    memset(visited, 0, rowSize * colSize * sizeof(uint8_t));
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (visited[i][j] == 0) {
                perimeter = 0, area = 0;
                regionDFS(rowSize, colSize, gardenPlots2D, visited, i, j, &perimeter, &area);
                price += area * perimeter;
            }
        }
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %u\n", price);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    price = 0;
    uint8_t visitedSides[rowSize][colSize][5], BFSQueue[size][2];
    uint32_t sides, queueStart = 0, queueEnd = 0, row, col;
    memset(BFSQueue, 0, size * 2 * sizeof(uint8_t));
    memset(visitedSides, 0, rowSize * colSize * 5 * sizeof(uint8_t));
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            if (visitedSides[i][j][0] == 0) { // 28 * 12
                sides = 0, area = 0, BFSQueue[queueEnd][0] = i, BFSQueue[queueEnd][1] = j, queueEnd = (queueEnd + 1) % size;
                while (queueEnd != queueStart) {
                    row = BFSQueue[queueStart][0], col = BFSQueue[queueStart][1], queueStart = (queueStart + 1) % size;
                    if (visitedSides[row][col][0] == 1) continue;
                    area++;
                    visitedSides[row][col][0] = 1;
                    if (row == 0 || gardenPlots2D[row - 1][col] != gardenPlots2D[row][col]) {
                        sides += checkNewSide(rowSize, colSize, gardenPlots2D, visitedSides, row, col, UP);
                    }
                    if (row == rowSize - 1 || gardenPlots2D[row + 1][col] != gardenPlots2D[row][col]) {
                        sides += checkNewSide(rowSize, colSize, gardenPlots2D, visitedSides, row, col, DOWN);
                    }
                    if (col == 0 || gardenPlots2D[row][col - 1] != gardenPlots2D[row][col]) {
                        sides += checkNewSide(rowSize, colSize, gardenPlots2D, visitedSides, row, col, LEFT);
                    }
                    if (col == colSize - 1 || gardenPlots2D[row][col + 1] != gardenPlots2D[row][col]) {
                        sides += checkNewSide(rowSize, colSize, gardenPlots2D, visitedSides, row, col, RIGHT);
                    }
                    if (row != 0 && visitedSides[row - 1][col][0] == 0 && gardenPlots2D[row - 1][col] == gardenPlots2D[row][col]) { // UP
                        BFSQueue[queueEnd][0] = row - 1, BFSQueue[queueEnd][1] = col, queueEnd = (queueEnd + 1) % size;
                    }
                    if (row != rowSize - 1 && visitedSides[row + 1][col][0] == 0 && gardenPlots2D[row + 1][col] == gardenPlots2D[row][col]) { // DOWN
                        BFSQueue[queueEnd][0] = row + 1, BFSQueue[queueEnd][1] = col, queueEnd = (queueEnd + 1) % size;
                    }
                    if (col != 0 && visitedSides[row][col - 1][0] == 0 && gardenPlots2D[row][col - 1] == gardenPlots2D[row][col]) { // LEFT
                        BFSQueue[queueEnd][0] = row, BFSQueue[queueEnd][1] = col - 1, queueEnd = (queueEnd + 1) % size;
                    }
                    if (col != colSize - 1 && visitedSides[row][col + 1][0] == 0 && gardenPlots2D[row][col + 1] == gardenPlots2D[row][col]) { // RIGHT
                        BFSQueue[queueEnd][0] = row, BFSQueue[queueEnd][1] = col + 1, queueEnd = (queueEnd + 1) % size;
                    }
                }
                price += area * sides;
            }
        }
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %u\n", price);
    printf("Part 2 Time: %ld us\n", usTime);

    free(gardenPlots);

    return 0;
}

void regionDFS(uint32_t rowSize, uint32_t colSize, uint8_t (*gardenPlots2D)[colSize], uint8_t (*visited)[colSize], int row, int col, uint32_t* perimeter, uint32_t* area) {
    (*area)++;
    visited[row][col] = 1;
    if (row == 0 || gardenPlots2D[row - 1][col] != gardenPlots2D[row][col]) { // UP
        (*perimeter)++;
    } else {
        if (row != 0 && visited[row - 1][col] == 0) regionDFS(rowSize, colSize, gardenPlots2D, visited, row - 1, col, perimeter, area);
    }
    if (row == rowSize - 1 || gardenPlots2D[row + 1][col] != gardenPlots2D[row][col]) { // DOWN
        (*perimeter)++;
    } else {
        if (row != rowSize - 1 && visited[row + 1][col] == 0) regionDFS(rowSize, colSize, gardenPlots2D, visited, row + 1, col, perimeter, area);
    }
    if (col == 0 || gardenPlots2D[row][col - 1] != gardenPlots2D[row][col]) { // LEFT
        (*perimeter)++;
    } else {
        if (col != 0 && visited[row][col - 1] == 0) regionDFS(rowSize, colSize, gardenPlots2D, visited, row, col - 1, perimeter, area);
    }
    if (col == colSize - 1 || gardenPlots2D[row][col + 1] != gardenPlots2D[row][col]) { // RIGHT
        (*perimeter)++;
    } else {
        if (col != colSize - 1 && visited[row][col + 1] == 0) regionDFS(rowSize, colSize, gardenPlots2D, visited, row, col + 1, perimeter, area);
    }
}

const int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
uint8_t checkNewSide(uint32_t rowSize, uint32_t colSize, uint8_t (*gardenPlots2D)[colSize], uint8_t (*visitedSides)[colSize][5], int row, int col, uint32_t side) {
    visitedSides[row][col][side] = 1;
    uint8_t newSide = 1;
    uint32_t size = side < LEFT ? rowSize : colSize;
    int newRow, newCol;
    for (int i = 1; i < size; i++) {
        newRow = row + i * directions[side - 1][0], newCol = col + i * directions[side - 1][1];
        if (newRow < 0 || newRow >= rowSize || newCol < 0 || newCol >= colSize) break;
        if (gardenPlots2D[newRow][newCol] != gardenPlots2D[row][col]) break;
        int newRow2 = newRow + directions[(side + 1) % 4][0], newCol2 = newCol + directions[(side + 1) % 4][1];
        if (newRow2 >= 0 && newRow2 < rowSize && newCol2 >= 0 && newCol2 < colSize) {
            if (gardenPlots2D[newRow2][newCol2] == gardenPlots2D[newRow][newCol]) break;
        }
        if (visitedSides[newRow][newCol][side] == 1) {
            newSide = 0;
            break;
        }
    }
    if (newSide == 0) return 0;
    for (int i = 1; i < size; i++) {
        newRow = row + -1 * i * directions[side - 1][0], newCol = col + -1 * i * directions[side - 1][1];
        if (newRow < 0 || newRow >= rowSize || newCol < 0 || newCol >= colSize) break;
        if (gardenPlots2D[newRow][newCol] != gardenPlots2D[row][col]) break;
        int newRow2 = newRow + directions[(side + 1) % 4][0], newCol2 = newCol + directions[(side + 1) % 4][1];
        if (newRow2 >= 0 && newRow2 < rowSize && newCol2 >= 0 && newCol2 < colSize) {
            if (gardenPlots2D[newRow2][newCol2] == gardenPlots2D[newRow][newCol]) break;
        }
        if (visitedSides[newRow][newCol][side] == 1) {
            newSide = 0;
            break;
        }
    }
    return newSide;
}