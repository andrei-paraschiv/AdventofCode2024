#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
    size = size / 2;
    fclose(fptr);

    // Part 1 - O(n*log(n))   
    qsort(nums1, size, sizeof(int), compare_function);
    qsort(nums2, size, sizeof(int), compare_function);

    int totalDiff = 0;
    for (int i = 0; i < size; i++) {
        totalDiff += abs(nums1[i] - nums2[i]);
    }

    printf("Part 1 Solution: %d\n", totalDiff);

    // Part 2 - O(n)
    int smallest = nums1[0], largest = nums1[0];
    for (int i = 1; i < size; i++) {
        if (nums1[i] < smallest) smallest = nums1[i];
        if (nums1[i] > largest) largest = nums1[i];
    }

    uint32_t* occurences = malloc((largest - smallest + 1) * sizeof(uint32_t));
    for (int i = 0; i < size; i++) {
        if (nums2[i] >= smallest && nums2[i] <= largest) {
            occurences[nums2[i] - smallest]++;
        }
    }

    int similarityScore = 0;
    for (int i = 0; i < size; i++) {
        similarityScore += occurences[nums1[i] - smallest] * nums1[i];
    }

    printf("Part 2 Solution: %d\n", similarityScore);

    free(nums1);
    free(nums2);
    free(occurences);

    return 0;
}

int compare_function(const void *a, const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}