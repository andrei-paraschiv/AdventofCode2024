#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 101
#define HEIGHT 103
#define TIME 100

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MOD(x, y) ((x) < 0 ? (y) - (ABS(x) % (y)) : (x) % (y))

struct Robot {
    int8_t pX, pY, vX, vY;
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

    int size = 0, initSize = 1000, c;
    int8_t val = 0, valCount = 0, sign = 1; 
    struct Robot* robots = malloc(initSize * sizeof(struct Robot));
    while (1) {
        c = fgetc(fptr);
        if (c >= '0' && c <= '9' || c == '-') {
            if (c == '-') {
                sign = -1;
            } else {
                val = val * 10 + sign * (c - '0');
            }
        } else if (c == ',' || c == ' ' || c == '\n' || c == EOF) {
            if (valCount == 0) {
                if (size == initSize) {
                    initSize *= 2;
                    robots = realloc(robots, initSize * sizeof(struct Robot));
                }
                robots[size].pX = val;
            } else if (valCount == 1) {
                robots[size].pY = val;
            } else if (valCount == 2) {
                robots[size].vX = val;
            } else {
                robots[size].vY = val;
                size++;
            }
            val = 0;
            valCount = (valCount + 1) % 4;
            sign = 1;
        }
        if (c == EOF) break;
    }
    fclose(fptr);

    // Part 1 - O(n)
    clock_t startTime = clock();

    uint32_t safetyFactor = 0, quad1 = 0, quad2 = 0, quad3 = 0, quad4 = 0;
    uint8_t finalX, finalY;
    for (int i = 0; i < size; i++) {
        finalX = MOD((robots[i].pX + TIME * robots[i].vX), WIDTH);
        finalY = MOD((robots[i].pY + TIME * robots[i].vY), HEIGHT);
        if (finalX == WIDTH) finalX = 0;
        if (finalY == HEIGHT) finalY = 0;
        if (finalX < WIDTH / 2 && finalY < HEIGHT / 2) {
            quad1++;
        } else if (finalX > WIDTH / 2 && finalY < HEIGHT / 2) {
            quad2++;
        } else if (finalX < WIDTH / 2 && finalY > HEIGHT / 2) {
            quad3++;
        } else if (finalX > WIDTH / 2 && finalY > HEIGHT / 2) {
            quad4++;
        }
    }
    safetyFactor = quad1 * quad2 * quad3 * quad4;

    int64_t usTime = clock() - startTime;

    printf("Part 1 Solution: %u\n", safetyFactor);
    printf("Part 1 Time: %ld us\n", usTime);

    // Part 2 - O(n)
    startTime = clock();

    uint32_t time = 1;
    uint8_t map[WIDTH][HEIGHT] = {0};
    uint32_t adjacency = 0;
    while(1) {
        for (int i = 0; i < size; i++) {
            robots[i].pX = MOD((robots[i].pX + robots[i].vX), WIDTH);
            robots[i].pY = MOD((robots[i].pY + robots[i].vY), HEIGHT);
            map[robots[i].pX][robots[i].pY] = 1;
            if (robots[i].pX != 0 && map[robots[i].pX - 1][robots[i].pY] == 1) {
                adjacency++;
            }
        }
        if (adjacency > 45) break;
        adjacency = 0;
        time++;
        memset(map, 0, WIDTH * HEIGHT * sizeof(uint8_t));
    }

    // Print the Christmas tree:
    // for (int i = 0; i < WIDTH; i++) {
    //     for (int j = 0; j < HEIGHT; j++) {
    //         if (map[j][i] == 1) {
    //             printf("#");
    //         } else {
    //             printf(".");
    //         }
    //     }
    //     printf("\n");
    // }

    usTime = clock() - startTime;

    printf("Part 2 Solution: %u\n", time);
    printf("Part 2 Time: %ld us\n", usTime);

    free(robots);

    return 0;
}