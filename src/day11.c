#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct HashBucket {
    uint64_t* keys;
    uint64_t* values;
    uint32_t size, initSize; 
};

struct HashTable {
    struct HashBucket buckets[1009];
    uint32_t size;
};

void initHashTable(struct HashTable* hashTable);
void insertHashTable(struct HashTable* hashTable, uint64_t key, uint64_t value);
void updateHashTableValues(struct HashTable* hashTablePrev, struct HashTable* hashTable);
uint64_t getTotalHashTableValues(struct HashTable* hashTable);
void freeHashTable(struct HashTable* hashTable);

uint8_t numDigits(int64_t a);

const int64_t power10[19] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
    100000000, 1000000000, 10000000000, 100000000000,
    1000000000000, 10000000000000, 100000000000000,
    1000000000000000, 10000000000000000, 100000000000000000,
    1000000000000000000
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

    int num = 0, c;
    uint32_t size = 0, initSize = 10; 
    uint64_t* stones = malloc(initSize * sizeof(uint64_t));
    while (1) {
        c = fgetc(fptr);
        if (c == ' ' || c == EOF) {
            if (size == initSize) {
                initSize *= 2;
                stones = realloc(stones, initSize * sizeof(uint64_t));
            }
            stones[size] = num;
            num = 0;
            size++;
            if (c == EOF) break;
        } else {
            num = num * 10 + c - '0';
        }
    }
    fclose(fptr);

    // Part 1 - O(n^2) (O(n) average)
    clock_t startTime = clock();

    struct HashTable hashTable[2];
    initHashTable(&hashTable[0]);
    initHashTable(&hashTable[1]);

    for (int i = 0; i < size; i++) {
        insertHashTable(&hashTable[0], stones[i], 1);
    }

    for (int i = 0; i < 25; i++) {
        updateHashTableValues(&hashTable[i % 2], &hashTable[(i + 1) % 2]);
    }

    uint64_t numStones = getTotalHashTableValues(&hashTable[25 % 2]);    

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %lu\n", numStones);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n^2) (O(n) average)
    startTime = clock();

    for (int i = 25; i < 75; i++) {
        updateHashTableValues(&hashTable[i % 2], &hashTable[(i + 1) % 2]);
    }

    numStones = getTotalHashTableValues(&hashTable[75 % 2]);    

    usTime = clock() - startTime;

    printf("Part 2 Solution: %lu\n", numStones);
    printf("Part 2 Time: %ld us\n", usTime);

    free(stones);
    freeHashTable(&hashTable[0]);
    freeHashTable(&hashTable[1]);

    return 0;
}

uint8_t numDigits(int64_t a) {
    int numDigits = 0;
    if (a < 10000000000) { // 1 - 10
        if (a < 100000) { // 1 - 5
            if (a < 100) { // 1 - 2
                if (a < 10) {
                    numDigits = 1;
                } else {
                    numDigits = 2;
                }
            } else { // 3 - 5
                if (a < 1000) {
                    numDigits = 3;
                } else if (a < 10000) {
                    numDigits = 4;
                } else {
                    numDigits = 5;
                }
            }
        } else { // 6 - 10
            if (a < 10000000) { // 6 - 7
                if (a < 1000000) {
                    numDigits = 6;
                } else {
                    numDigits = 7;
                }
            } else { // 8 - 10
                if (a < 100000000) {
                    numDigits = 8;
                } else if (a < 1000000000) {
                    numDigits = 9;
                } else {
                    numDigits = 10;
                }
            }
        }
    } else { // 11 - 19
        if (a < 1000000000000000) { // 11 - 15
            if (a < 1000000000000) { // 11 - 12
                if (a < 100000000000) {
                    numDigits = 11;
                } else {
                    numDigits = 12;
                }
            } else { // 13 - 15
                if (a < 10000000000000) {
                    numDigits = 13;
                } else if (a < 100000000000000) {
                    numDigits = 14;
                } else {
                    numDigits = 15;
                }
            }
        } else { // 16 - 19
            if (a < 100000000000000000) { // 16 - 17
                if (a < 10000000000000000) {
                    numDigits = 16;
                } else {
                    numDigits = 17;
                }
            } else { // 18 - 19
                if (a < 1000000000000000000) {
                    numDigits = 18;
                } else {
                    numDigits = 19;
                }
            }
        }
    }
    return numDigits;
}

void initHashTable(struct HashTable* hashTable) {
    hashTable->size = 1009;
    for (int i = 0; i < 1009; i++) {
        hashTable->buckets[i].size = 0;
        hashTable->buckets[i].initSize = 10;
        hashTable->buckets[i].keys = malloc(hashTable->buckets[i].initSize * sizeof(uint64_t));
        hashTable->buckets[i].values = malloc(hashTable->buckets[i].initSize * sizeof(uint64_t));
    }
}

void insertHashTable(struct HashTable* hashTable, uint64_t key, uint64_t value) {
    uint32_t index = key % 1009, bucketIndex = 0;
    struct HashBucket* bucket = &hashTable->buckets[index];
    for (int i = 0; i < bucket->size; i++) {
        if (bucket->keys[bucketIndex] == key) {
            bucket->values[i] += value;
            return;
        }
    }
    if (bucket->size == bucket->initSize) {
        bucket->initSize *= 2;
        bucket->keys = realloc(bucket->keys, bucket->initSize * sizeof(uint64_t));
        bucket->values = realloc(bucket->values, bucket->initSize * sizeof(uint64_t));
    }
    bucket->keys[bucket->size] = key;
    bucket->values[bucket->size] = value;
    bucket->size++;
}

void updateHashTableValues(struct HashTable* hashTablePrev, struct HashTable* hashTable) {
    uint8_t numDigitsVal;
    uint64_t value;
    uint64_t key;
    for (int i = 0; i < hashTablePrev->size; i++) {
        for (int j = 0; j < hashTablePrev->buckets[i].size; j++) {
            value = hashTablePrev->buckets[i].values[j];
            if (value == 0) continue;
            key = hashTablePrev->buckets[i].keys[j];
            numDigitsVal = numDigits(key);
            if (key == 0) {
                insertHashTable(hashTable, 1, value);
            } else if (numDigitsVal % 2 == 0) {
                insertHashTable(hashTable, key / power10[numDigitsVal / 2], value);
                insertHashTable(hashTable, key % power10[numDigitsVal / 2], value);
                
            } else {
                insertHashTable(hashTable, 2024 * key, value);
            }
            hashTablePrev->buckets[i].values[j] = 0;
        }
    }
}

uint64_t getTotalHashTableValues(struct HashTable* hashTable) {
    uint64_t totalValue = 0;
    for (int i = 0; i < hashTable->size; i++) {
        for (int j = 0; j < hashTable->buckets[i].size; j++) {
            totalValue += hashTable->buckets[i].values[j];
        }
    }
    return totalValue;
}

void freeHashTable(struct HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        free(hashTable->buckets[i].keys);
        free(hashTable->buckets[i].values);
    }
}