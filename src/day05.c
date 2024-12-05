#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Update {
    int* pages;
    uint32_t size;
};

struct ExtraCompareArguments {
    int min, max;
    int* rules;
};

struct ExtraCompareArguments extraCompareArguments;

int compare_function(const void *a, const void *b);

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

    int rulesSize = 0, rulesInitSize = 1000, num = 0, c, prevC, min, max, first = 1;
    int* rules = malloc(rulesInitSize * sizeof(int));
    while (1) {
        c = fgetc(fptr);
        if (c == '\n' || c == '|') {
            if (rulesSize == rulesInitSize) {
                rulesInitSize *= 2;
                rules = realloc(rules, rulesInitSize * sizeof(int));
            }
            if (num) {
                rules[rulesSize] = num;
                if (first) {
                    min = num;
                    max = min;
                    first = 0;
                } else {
                    if (min > num) min = num;
                    if (max < num) max = num;
                }
                rulesSize++;
                num = 0;
            }
            if (prevC == '\n') break;
        } else {
            num = num * 10 + c - '0';
        }
        prevC = c;
    }

    int updatesSize = 0, updatesInitSize = 100, pagesInitSize = 10;
    struct Update* updates = malloc(updatesInitSize * sizeof(struct Update));
    updates[updatesSize].pages = malloc(pagesInitSize * sizeof(int));
    updates[updatesSize].size = 0;
    while (1) {
        c = fgetc(fptr);
        if (c == '\n' || c == ',' || c == EOF) {
            if (updates[updatesSize].size == pagesInitSize) {
                pagesInitSize *= 2;
                updates[updatesSize].pages = realloc(updates[updatesSize].pages, pagesInitSize * sizeof(int));
            }
            updates[updatesSize].pages[updates[updatesSize].size] = num;
            updates[updatesSize].size++;
            if (c == '\n') {
                pagesInitSize = 10;
                updatesSize++;
                if (updatesSize == updatesInitSize) {
                    updatesInitSize *= 2;
                    updates = realloc(updates, updatesInitSize * sizeof(struct Update));
                }
                updates[updatesSize].pages = malloc(pagesInitSize * sizeof(int));
                updates[updatesSize].size = 0;
            }
            num = 0;
            if (c == EOF) {
                updatesSize++;
                break;
            }
        } else {
            num = num * 10 + c - '0';
        }
    }
    fclose(fptr);

    // Part 1 - O(r + u) where r is the number of rules and u is the total size of all updates
    int rules2D[max - min + 1][max - min + 1];
    memset(rules2D, 0, sizeof(rules2D));
    for (int i = 0; i < rulesSize / 2; i++) {
        rules2D[rules[2 * i] - min][rules[2 * i + 1] - min] = 1;
        rules2D[rules[2 * i + 1] - min][rules[2 * i] - min] = -1;
    }

    int sum = 0, valid;
    for (int i = 0; i < updatesSize; i++) {
        valid = 1;
        for (int j = 1; j < updates[i].size; j++) {
            if (rules2D[updates[i].pages[j - 1] - min][updates[i].pages[j] - min] != 1) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            sum += updates[i].pages[updates[i].size / 2];
        }
    }

    printf("Part 1 Solution: %d\n", sum);

    // Part 2 - O(r + u * log(u)) where r is the number of rules and u is the total size of all updates
    sum = 0;
    extraCompareArguments.min = min;
    extraCompareArguments.max = max;
    extraCompareArguments.rules = (int*)rules2D;
    for (int i = 0; i < updatesSize; i++) {
        for (int j = 1; j < updates[i].size; j++) {
            if (rules2D[updates[i].pages[j - 1] - min][updates[i].pages[j] - min] != 1) {
                qsort(updates[i].pages, updates[i].size, sizeof(int), compare_function);
                sum += updates[i].pages[updates[i].size / 2];
                break;
            }
        }
    }

    printf("Part 2 Solution: %d\n", sum);

    free(rules);
    for (int i = 0; i < updatesSize; i++) {
        free(updates[i].pages);
    }
    free(updates);

    return 0;
}

int compare_function(const void *a, const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    int size = extraCompareArguments.max - extraCompareArguments.min + 1;
    int (*rules)[size] = (int(*)[size])extraCompareArguments.rules;
    int min = extraCompareArguments.min;
    return rules[*y - min][*x - min];
}