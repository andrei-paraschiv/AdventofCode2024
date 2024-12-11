#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Location {
    uint32_t row;
    uint32_t col;
};

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

    int size = 0, colSize = 0, rowSize = 1, initSize = 1000, c; 
    uint8_t* map = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (size == initSize) {
            initSize *= 2;
            map = realloc(map, initSize * sizeof(uint8_t));
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

    // Part 1 - O(n^2)
    clock_t startTime = clock();

    int numAntinodes = 0;
    uint32_t antinodeRow, antinodeCol;
    uint8_t frequency;
    uint8_t (*map2D)[colSize] = (uint8_t (*)[colSize]) map;
    struct Location** visitedAntennas = malloc(UINT8_MAX * sizeof(struct Location*));
    for (int i = 0; i < UINT8_MAX; i++) visitedAntennas[i] = malloc(sizeof(struct Location));
    uint32_t visitedAntennasSize[UINT8_MAX] = {0};
    uint8_t visitedAntinodes[rowSize][colSize];
    memset(visitedAntinodes, 0, rowSize * colSize * sizeof(uint8_t));
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            frequency = map2D[i][j];
            if (frequency != '.') {
                for (int k = 0; k < visitedAntennasSize[frequency]; k++) {
                    antinodeRow = 2 * i - visitedAntennas[frequency][k].row;
                    antinodeCol = 2 * j - visitedAntennas[frequency][k].col;
                    if (antinodeRow >= 0 && antinodeRow < rowSize && antinodeCol >= 0 && antinodeCol < colSize && visitedAntinodes[antinodeRow][antinodeCol] == 0) {
                        numAntinodes++;
                        visitedAntinodes[antinodeRow][antinodeCol] = 1;
                    }
                    antinodeRow = 2 * visitedAntennas[frequency][k].row - i;
                    antinodeCol = 2 * visitedAntennas[frequency][k].col - j;
                    if (antinodeRow >= 0 && antinodeRow < rowSize && antinodeCol >= 0 && antinodeCol < colSize && visitedAntinodes[antinodeRow][antinodeCol] == 0) {
                        numAntinodes++;
                        visitedAntinodes[antinodeRow][antinodeCol] = 1;
                    }
                }
                visitedAntennasSize[frequency]++;
                if (visitedAntennasSize[frequency] > 1) visitedAntennas[frequency] = realloc(visitedAntennas[frequency], visitedAntennasSize[frequency] * sizeof(uint64_t));
                visitedAntennas[frequency][visitedAntennasSize[frequency] - 1] = (struct Location){i, j};
            }
        }
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %d\n", numAntinodes);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n^2)
    startTime = clock();

    int directionRow, directionCol;
    numAntinodes = 0;
    for (int i = 0; i < UINT8_MAX; i++) visitedAntennas[i] = realloc(visitedAntennas[i], sizeof(struct Location));
    memset(visitedAntennasSize, 0, UINT8_MAX * sizeof(uint32_t));
    memset(visitedAntinodes, 0, rowSize * colSize * sizeof(uint8_t));
    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            frequency = map2D[i][j];
            if (frequency != '.') {
                for (int k = 0; k < visitedAntennasSize[frequency]; k++) {
                    directionRow = i - visitedAntennas[frequency][k].row;
                    directionCol = j - visitedAntennas[frequency][k].col;
                    antinodeRow = i;
                    antinodeCol = j;
                    while(1) {
                        if (antinodeRow < 0 || antinodeRow >= rowSize || antinodeCol < 0 || antinodeCol >= colSize) break;
                        if (visitedAntinodes[antinodeRow][antinodeCol] == 0) {
                            numAntinodes++;
                            visitedAntinodes[antinodeRow][antinodeCol] = 1;
                        }
                        antinodeRow += directionRow;
                        antinodeCol += directionCol;
                    }
                    antinodeRow = i - directionRow;
                    antinodeCol = j - directionCol;
                    while(1) {
                        if (antinodeRow < 0 || antinodeRow >= rowSize || antinodeCol < 0 || antinodeCol >= colSize) break;
                        if (visitedAntinodes[antinodeRow][antinodeCol] == 0) {
                            numAntinodes++;
                            visitedAntinodes[antinodeRow][antinodeCol] = 1;
                        }
                        antinodeRow -= directionRow;
                        antinodeCol -= directionCol;
                    }
                }
                visitedAntennasSize[frequency]++;
                if (visitedAntennasSize[frequency] > 1) visitedAntennas[frequency] = realloc(visitedAntennas[frequency], visitedAntennasSize[frequency] * sizeof(uint64_t));
                visitedAntennas[frequency][visitedAntennasSize[frequency] - 1] = (struct Location){i, j};
            }
        }
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %d\n", numAntinodes);
    printf("Part 2 Time: %ld us\n", usTime);

    free(map);
    for (int i = 0; i < UINT8_MAX; i++) free(visitedAntennas[i]);
    free(visitedAntennas);

    return 0;
}