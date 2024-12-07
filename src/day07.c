#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Equation {
    int64_t* nums;
    uint32_t size;
};

int64_t concatenate(int64_t a, int64_t b);

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

    int size = 0, initSize = 1000, equationInitSize = 10, c, maxEquationSize = 0;
    int64_t num = 0;
    struct Equation* equations = malloc(initSize * sizeof(struct Equation));
    equations[size].size = 0;
    equations[size].nums = malloc(equationInitSize * sizeof(int64_t));
    while (1) {
        c = fgetc(fptr);
        if (size == initSize) {
            initSize *= 2;
            equations = realloc(equations, initSize * sizeof(struct Equation));
        }
        if (c == ':' || c == '\n' || c == ' ' || c == EOF) {
            if (num) {
                if (equations[size].size == equationInitSize) {
                    equationInitSize *= 2;
                    equations[size].nums = realloc(equations[size].nums, equationInitSize * sizeof(int64_t));
                }
                equations[size].nums[equations[size].size] = num;
                equations[size].size++;
                num = 0;
            } 
            if (c == '\n' || c == EOF) {
                if (equations[size].size > maxEquationSize) maxEquationSize = equations[size].size;
                size++;
                equationInitSize = 10;
                if (c == EOF) break;
                equations[size].size = 0;
                equations[size].nums = malloc(equationInitSize * sizeof(int64_t));
            }
        } else {
            num = num * 10 + c - '0';
        }
    }
    fclose(fptr);    
    
    // Part 1 - O(n * 2^n)
    int64_t totalResult = 0, equationValue, totalArrangements;
    for (int i = 0; i < size; i++) {
        totalArrangements = 1 << (equations[i].size - 2);
        for (int j = 0; j < totalArrangements; j++) {
            equationValue = equations[i].nums[1];
            for (int k = 0; k < equations[i].size - 2; k++) {
                if (j >> k & 1) {
                    equationValue += equations[i].nums[k + 2];
                } else {
                    equationValue *= equations[i].nums[k + 2];
                }
            }
            if (equationValue == equations[i].nums[0]) {
                totalResult += equationValue;
                break;
            }
        }
    }

    printf("Part 1 Solution: %ld\n", totalResult);

    // Part 2 - O(n * 3^n)
    int64_t pow3[maxEquationSize - 1];
    for (int i = 0; i < maxEquationSize - 1; i++) {
        pow3[i] = 1;
        for (int j = 0; j < i; j++) {
            pow3[i] *= 3;
        }
    }

    totalResult = 0;
    int64_t operationSelect;
    for (int i = 0; i < size; i++) {
        totalArrangements = pow3[equations[i].size - 2];
        for (int j = 0; j < totalArrangements; j++) {
            equationValue = equations[i].nums[1];
            operationSelect = j;
            for (int k = 0; k < equations[i].size - 2; k++) {
                if (operationSelect % 3 == 0) {
                    equationValue += equations[i].nums[k + 2];
                } else if (operationSelect % 3 == 1) {
                    equationValue *= equations[i].nums[k + 2];
                } else {
                    equationValue = concatenate(equationValue, equations[i].nums[k + 2]);
                    if (equationValue == 0) break;
                }
                operationSelect /= 3;
            }
            if (equationValue == equations[i].nums[0]) {
                totalResult += equationValue;
                break;
            }
        }
    }

    printf("Part 2 Solution: %ld\n", totalResult);

    for (int i = 0; i < size; i++) {
        free(equations[i].nums);
    }
    free(equations);

    return 0;
}

const int64_t power10[19] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
    100000000, 1000000000, 10000000000, 100000000000,
    1000000000000, 10000000000000, 100000000000000,
    1000000000000000, 10000000000000000, 100000000000000000,
    1000000000000000000
};

int64_t concatenate(int64_t a, int64_t b) {
    int numDigits = 0;
    if (b < 10000000000) { // 1 - 10
        if (b < 100000) { // 1 - 5
            if (b < 100) { // 1 - 2
                if (b < 10) {
                    numDigits = 1;
                } else {
                    numDigits = 2;
                }
            } else { // 3 - 5
                if (b < 1000) {
                    numDigits = 3;
                } else if (b < 10000) {
                    numDigits = 4;
                } else {
                    numDigits = 5;
                }
            }
        } else { // 6 - 10
            if (b < 10000000) { // 6 - 7
                if (b < 1000000) {
                    numDigits = 6;
                } else {
                    numDigits = 7;
                }
            } else { // 8 - 10
                if (b < 100000000) {
                    numDigits = 8;
                } else if (b < 1000000000) {
                    numDigits = 9;
                } else {
                    numDigits = 10;
                }
            }
        }
    } else { // 11 - 19
        if (b < 1000000000000000) { // 11 - 15
            if (b < 1000000000000) { // 11 - 12
                if (b < 100000000000) {
                    numDigits = 11;
                } else {
                    numDigits = 12;
                }
            } else { // 13 - 15
                if (b < 10000000000000) {
                    numDigits = 13;
                } else if (b < 100000000000000) {
                    numDigits = 14;
                } else {
                    numDigits = 15;
                }
            }
        } else { // 16 - 19
            if (b < 100000000000000000) { // 16 - 17
                if (b < 10000000000000000) {
                    numDigits = 16;
                } else {
                    numDigits = 17;
                }
            } else { // 18 - 19
                if (b < 1000000000000000000) {
                    numDigits = 18;
                } else {
                    numDigits = 19;
                }
            }
        }
    }
    int64_t result = a * power10[numDigits] + b;
    if (numDigits == 19 || result < 0) return 0;
    return result;
}