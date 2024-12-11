#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Report {
    int* levels;
    uint32_t size;
};

uint32_t getUnsafeIndex(int* levels, uint32_t size);
void removeIndex(int* levels, uint32_t size, uint32_t index, int* newLevels);

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

    int size = 0, level = 0, initSize = 1000, reportInitSize = 10, c;
    struct Report* reports = malloc(initSize * sizeof(struct Report));
    reports[size].size = 0;
    reports[size].levels = malloc(reportInitSize * sizeof(int));
    while (1) {
        c = fgetc(fptr);
        if (c == '\n' || c == ' ' || c == EOF) {
            if (level != 0) {
                if (reports[size].size == reportInitSize) {
                    reportInitSize *= 2;
                    reports[size].levels = realloc(reports[size].levels, reportInitSize * sizeof(int));
                }
                reports[size].levels[reports[size].size] = level;
                reports[size].size++;
                
                if (c == '\n') {
                    size++;
                    if (size == initSize) {
                        initSize *= 2;
                        reports = realloc(reports, initSize * sizeof(struct Report));
                    }
                    reportInitSize = 10;
                    reports[size].size = 0;
                    reports[size].levels = malloc(reportInitSize * sizeof(int));
                }

                if (c == EOF) {
                    size++;
                    break;
                }
                level = 0;
            }
        } else {
            level = level * 10 + c - '0';
        }
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    int safe = size;
    for (int i = 0; i < size; i++) {
        if (getUnsafeIndex(reports[i].levels, reports[i].size)) safe--;
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %d\n", safe);
    printf("Part 1 Time: %ld us\n", usTime);

    //Part 2 - O(n)
    startTime = clock();

    safe = size;
    int* removedIndex = 0, unsafeIndex;
    for (int i = 0; i < size; i++) {
        if (removedIndex) {
            free(removedIndex);
            removedIndex = 0;
        }
        if (unsafeIndex = getUnsafeIndex(reports[i].levels, reports[i].size)) {
            removedIndex = malloc((reports[i].size - 1) * sizeof(int));
            removeIndex(reports[i].levels, reports[i].size, unsafeIndex, removedIndex);
            if (!getUnsafeIndex(removedIndex, reports[i].size - 1)) continue;
            removeIndex(reports[i].levels, reports[i].size, unsafeIndex - 1, removedIndex);
            if (!getUnsafeIndex(removedIndex, reports[i].size - 1)) continue;
            removeIndex(reports[i].levels, reports[i].size, 0, removedIndex);
            if (!getUnsafeIndex(removedIndex, reports[i].size - 1)) continue;
            safe--;
        }
    }
    
    usTime = clock() - startTime;

    printf("Part 2 Solution: %d\n", safe);
    printf("Part 2 Time: %ld us\n", usTime);

    if (removedIndex) free(removedIndex);
    for (int i = 0; i < size; i++) free(reports[i].levels);
    free(reports);

    return 0;
}

uint32_t getUnsafeIndex(int* levels, uint32_t size) {
    int increasing = 0;
    for (int j = 1; j < size; j++) {
        if (j == 1) {
            if (levels[j] > levels[j - 1]) {
                increasing = 1;
            }
        }
        int curr = levels[j], prev = levels[j - 1], diff = abs(curr - prev);
        if ((increasing && curr <= prev) || (!increasing && curr >= prev) || diff < 1 || diff > 3) {
            return j;
        }
    }
    return 0;
}

void removeIndex(int* levels, uint32_t size, uint32_t index, int* newLevels) {
    for (int i = 0; i < size; i++) {
        if (i < index) {
            newLevels[i] = levels[i];
        } else if (i > index) {
            newLevels[i - 1] = levels[i];
        }
    }
}