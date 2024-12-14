#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PUSH_A 3
#define PUSH_B 1
#define PRIZE_PART2(x) ((x) + 10000000000000)

struct Machine {
    uint32_t buttonA[2];
    uint32_t buttonB[2];
    uint32_t prize[2];
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

    int size = 0, val = 0, valCount = 0, initSize = 1000, c; 
    struct Machine* machines = malloc(initSize * sizeof(struct Machine));
    while (1) {
        c = fgetc(fptr);
        if (c >= '0' && c <= '9') {
            val = val * 10 + c - '0';
        } else if (val) {
            if (valCount == 0) {
                if (size == initSize) {
                    initSize *= 2;
                    machines = realloc(machines, initSize * sizeof(struct Machine));
                }
                machines[size].buttonA[0] = val;
            } else if (valCount == 1) {
                machines[size].buttonA[1] = val;
            } else if (valCount == 2) {
                machines[size].buttonB[0] = val;
            } else if (valCount == 3) {
                machines[size].buttonB[1] = val;
            } else if (valCount == 4) {
                machines[size].prize[0] = val;
            } else if (valCount == 5) {
                machines[size].prize[1] = val;
                size++;
            }
            val = 0;
            valCount = (valCount + 1) % 6;
        }
        if (c == EOF) break;
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    uint8_t sign1, sign2, sign3;
    uint64_t leastTotalTokens = 0, buttonAPresses, buttonBPresses;
    uint64_t equationParts[4];
    for (int i = 0; i < size; i++) {
        equationParts[0] = machines[i].buttonA[0] * machines[i].buttonB[1];
        equationParts[1] = machines[i].buttonA[1] * machines[i].buttonB[0];
        if (equationParts[0] == equationParts[1]) continue;
        if (equationParts[0] > equationParts[1]) {
            equationParts[2] = equationParts[0] - equationParts[1];
            sign1 = 1;
        } else {
            equationParts[2] = equationParts[1] - equationParts[0];
            sign1 = 0;
        }
        equationParts[0] = machines[i].prize[0] * machines[i].buttonB[1];
        equationParts[1] = machines[i].prize[1] * machines[i].buttonB[0];
        if (equationParts[0] > equationParts[1]) {
            equationParts[3] = equationParts[0] - equationParts[1];
            sign2 = 1;
        } else {
            equationParts[3] = equationParts[1] - equationParts[0];
            sign2 = 0;
        }
        if (equationParts[3] % equationParts[2]) continue;
        if (sign1 != sign2) continue;
        buttonAPresses = equationParts[3] / equationParts[2];
        equationParts[0] = machines[i].buttonB[0];
        equationParts[1] = machines[i].prize[0];
        equationParts[2] = machines[i].buttonA[0] * buttonAPresses;
        if (equationParts[2] > equationParts[1]) continue;
        equationParts[3] = equationParts[1] - equationParts[2];
        if (equationParts[3] % equationParts[0]) continue;
        buttonBPresses = equationParts[3] / equationParts[0];
        leastTotalTokens += buttonAPresses * PUSH_A + buttonBPresses * PUSH_B;
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %lu\n", leastTotalTokens);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    leastTotalTokens = 0;
    for (int i = 0; i < size; i++) {
        equationParts[0] = machines[i].buttonA[0] * machines[i].buttonB[1];
        equationParts[1] = machines[i].buttonA[1] * machines[i].buttonB[0];
        if (equationParts[0] == equationParts[1]) continue;
        if (equationParts[0] > equationParts[1]) {
            equationParts[2] = equationParts[0] - equationParts[1];
            sign1 = 1;
        } else {
            equationParts[2] = equationParts[1] - equationParts[0];
            sign1 = 0;
        }
        equationParts[0] = PRIZE_PART2(machines[i].prize[0]) * machines[i].buttonB[1];
        equationParts[1] = PRIZE_PART2(machines[i].prize[1]) * machines[i].buttonB[0];
        if (equationParts[0] > equationParts[1]) {
            equationParts[3] = equationParts[0] - equationParts[1];
            sign2 = 1;
        } else {
            equationParts[3] = equationParts[1] - equationParts[0];
            sign2 = 0;
        }
        if (equationParts[3] % equationParts[2]) continue;
        if (sign1 != sign2) continue;
        buttonAPresses = equationParts[3] / equationParts[2];
        equationParts[0] = machines[i].buttonB[0];
        equationParts[1] = PRIZE_PART2(machines[i].prize[0]);
        equationParts[2] = machines[i].buttonA[0] * buttonAPresses;
        if (equationParts[2] > equationParts[1]) continue;
        equationParts[3] = equationParts[1] - equationParts[2];
        if (equationParts[3] % equationParts[0]) continue;
        buttonBPresses = equationParts[3] / equationParts[0];
        leastTotalTokens += buttonAPresses * PUSH_A + buttonBPresses * PUSH_B;
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %lu\n", leastTotalTokens);
    printf("Part 2 Time: %ld us\n", usTime);

    free(machines);

    return 0;
}