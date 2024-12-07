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

    int size = 0, initSize = 1000, c;
    uint8_t* memory = malloc(initSize * sizeof(uint8_t));
    while ((c = fgetc(fptr)) != EOF) {
        if (size == initSize) {
            initSize *= 2;
            memory = realloc(memory, initSize * sizeof(uint8_t));
        }
        memory[size] = c;
        size++;
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    int mulIndex = 0, val1 = 0, val2 = 0, sum = 0;
    for (int i = 0; i < size; i++) {
        if (memory[i] == 'm' && memory[i + 1] == 'u' && memory[i + 2] == 'l' && memory[i + 3] == '(') {
            mulIndex++;
            i += 3;
        } else if (mulIndex == 1 && memory[i] >= '0' && memory[i] <= '9') {
            val1 = val1 * 10 + memory[i] - '0';
            if (memory[i + 1] == ',') {
                mulIndex++;
                i++;
            }
        } else if (mulIndex == 2 && memory[i] >= '0' && memory[i] <= '9') {
            val2 = val2 * 10 + memory[i] - '0';
            if (memory[i + 1] == ')') {
                sum += val1 * val2;
                mulIndex = 0;
                val1 = 0;
                val2 = 0;
                i++;
            }
        } else {
            mulIndex = 0;
            val1 = 0;
            val2 = 0;
        }
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %d\n", sum);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    mulIndex = 0, val1 = 0, val2 = 0, sum = 0;
    int enabled = 1;
    for (int i = 0; i < size; i++) {
        if (memory[i] == 'd' && memory[i + 1] == 'o' && memory[i + 2] == '(' && memory[i + 3] == ')') {
            enabled = 1;
        }
        if (memory[i] == 'd' && memory[i + 1] == 'o' && memory[i + 2] == 'n' && memory[i + 3] == '\'' && memory[i + 4] == 't' && memory[i + 5] == '(' && memory[i + 6] == ')') {
            enabled = 0;
        }
        if (enabled && memory[i] == 'm' && memory[i + 1] == 'u' && memory[i + 2] == 'l' && memory[i + 3] == '(') {
            mulIndex++;
            i += 3;
        } else if (mulIndex == 1 && memory[i] >= '0' && memory[i] <= '9') {
            val1 = val1 * 10 + memory[i] - '0';
            if (memory[i + 1] == ',') {
                mulIndex++;
                i++;
            }
        } else if (mulIndex == 2 && memory[i] >= '0' && memory[i] <= '9') {
            val2 = val2 * 10 + memory[i] - '0';
            if (memory[i + 1] == ')') {
                sum += val1 * val2;
                mulIndex = 0;
                val1 = 0;
                val2 = 0;
                i++;
            }
        } else {
            mulIndex = 0;
            val1 = 0;
            val2 = 0;
        }
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %d\n", sum);
    printf("Part 2 Time: %ld us\n", usTime);

    free(memory);

    return 0;
}