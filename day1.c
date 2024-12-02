#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int compare_function(const void *a, const void *b);

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* fptr;
    fptr = fopen(argv[1], "r");
    
    if (fptr == NULL) {
        printf("File not found\n");
        return 1;
    }

    int size = 0, num = 0, initSize = 1000, c;
    int* nums1 = malloc(initSize * sizeof(int));
    int* nums2 = malloc(initSize * sizeof(int));
    while((c = fgetc(fptr)) != EOF) {
        if (c == '\n' || c == ' ') {
            if (num != 0) {
                if (size == 2 * initSize) {
                    initSize *= 2;
                    nums1 = realloc(nums1, initSize * sizeof(int));
                    nums2 = realloc(nums2, initSize * sizeof(int));
                }
                if (size % 2 == 0) {
                    nums1[size / 2] = num;
                } else {
                    nums2[size / 2] = num;
                }
                size++;
                num = 0;
            }
        } else {
            num = num * 10 + c - '0';
        }
    }

    fclose(fptr);

    size = size / 2;
    qsort(nums1, size, sizeof(int), compare_function);
    qsort(nums2, size, sizeof(int), compare_function);

    int totalDiff = 0;
    for (int i = 0; i < size; i++) {
        totalDiff += abs(nums1[i] - nums2[i]);
    }

    printf("Total difference: %d\n", totalDiff);

    int similarityScore = 0, prevScore;
    for (int i = 0; i < size; i++) {
        if (i == 0 || nums1[i] != nums1[i - 1]) {
            prevScore = 0;
            for (int j = 0; j < size; j++) {
                if (nums2[j] == nums1[i]) {
                    prevScore++;
                }
            }
            prevScore *= nums1[i];
        }
        similarityScore += prevScore;
    }

    printf("Similarity score: %d\n", similarityScore);

    free(nums1);
    free(nums2);

    return 0;
}

int compare_function(const void *a, const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}