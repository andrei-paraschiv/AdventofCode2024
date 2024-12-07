#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Equation {
    int64_t* nums;
    uint32_t size;
};

uint8_t reverseSearch(int64_t* equation, int index, int64_t accumulatedVal, int operation, uint8_t enableConcat);
int64_t unconcatenate(int64_t a, int64_t b);

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
    
    // Part 1 - O(2^n)
    clock_t startTime = clock();

    uint8_t validEquation;
    int64_t totalResult = 0;
    for (int i = 0; i < size; i++) {
        validEquation = reverseSearch(equations[i].nums, equations[i].size - 1, equations[i].nums[0], 0, 0);
        if (validEquation == 0) {
            validEquation = reverseSearch(equations[i].nums, equations[i].size - 1, equations[i].nums[0], 1, 0);
        }
        if (validEquation) {
            totalResult += equations[i].nums[0];   
        }     
    }

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %ld\n", totalResult);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(3^n)
    startTime = clock();

    totalResult = 0;
    for (int i = 0; i < size; i++) {
        validEquation = reverseSearch(equations[i].nums, equations[i].size - 1, equations[i].nums[0], 0, 1);
        if (validEquation == 0) {
            validEquation = reverseSearch(equations[i].nums, equations[i].size - 1, equations[i].nums[0], 1, 1);
        }
        if (validEquation == 0) {
            validEquation = reverseSearch(equations[i].nums, equations[i].size - 1, equations[i].nums[0], 2, 1);
        }
        if (validEquation) {
            totalResult += equations[i].nums[0];   
        }     
    }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %ld\n", totalResult);
    printf("Part 2 Time: %ld us\n", usTime);

    for (int i = 0; i < size; i++) {
        free(equations[i].nums);
    }
    free(equations);

    return 0;
}

uint8_t reverseSearch(int64_t* equation, int index, int64_t accumulatedVal, int operation, uint8_t enableConcat) {
    if (index == 0) {
        if (accumulatedVal) return 0;
        return 1;
    }

    if (operation == 0) {
        accumulatedVal -= equation[index];
        if (accumulatedVal < 0) return 0;
    } else if (operation == 1) {
        if (equation[index] == 0) {
            if (accumulatedVal) return 0;
            return 1;
        }
        if (accumulatedVal % equation[index]) return 0;
        accumulatedVal /= equation[index];
    } else {
        accumulatedVal = unconcatenate(accumulatedVal, equation[index]);
        if (accumulatedVal == -1) return 0;
    }
    if (accumulatedVal == 0 && index == 2 && equation[1] != 0) return 0;

    uint8_t validEquation = reverseSearch(equation, index - 1, accumulatedVal, 0, enableConcat);
    if (validEquation == 0) {
        validEquation = reverseSearch(equation, index - 1, accumulatedVal, 1, enableConcat);
    }
    if (validEquation == 0 && enableConcat) {
        validEquation = reverseSearch(equation, index - 1, accumulatedVal, 2, enableConcat);
    }
    return validEquation;
}

const int64_t power10[19] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
    100000000, 1000000000, 10000000000, 100000000000,
    1000000000000, 10000000000000, 100000000000000,
    1000000000000000, 10000000000000000, 100000000000000000,
    1000000000000000000
};

int64_t unconcatenate(int64_t a, int64_t b) {
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
    int64_t result = a / power10[numDigits];
    if (a - result * power10[numDigits] != b) return -1;
    return result;
}