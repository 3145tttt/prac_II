#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

static inline int div2_up(int a) {
    return (a + 1)/2;
} 

static inline int div2_down(int a) {
    return (a - 1)/2;
} 

int
main(int argC, char *argV[]) {

    enum { BASE = 10, PHASE_COUNT = 4 };

    char *end;
    int row = strtol(argV[2], &end, BASE), col = strtol(argV[3], &end, BASE);
    int SIZE;

    if (__builtin_mul_overflow(row, col, &SIZE)) {
        fprintf(stderr, "overflow row * col\n");
        exit(1);
    }

    if (__builtin_mul_overflow(SIZE, sizeof(col), &SIZE)) {
        fprintf(stderr, "overflow size\n");
        exit(1);
    }

    int fd = open(argV[1], O_RDWR | O_CREAT | O_TRUNC, 0600);

    if (fd == -1) {
        fprintf(stderr, "open file error\n");
        exit(1);
    }

    if (ftruncate(fd, SIZE) == -1) {
        fprintf(stderr, "ftruncate error\n");
        close(fd);
        exit(1);
    }

    int *arr = (int *) mmap(NULL, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    if (arr == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(1);
    }

    int steps = 0;
    if (col == 1 || row == 1) {
        int MAX = col > row ? col : row;
        for (int i = 0; i < MAX; ++i) {
            arr[i] = i + 1;
        }
    } else {
        if (row > col) {
            steps = 2*col;
        } else {
            steps = 2*row - 1;
        }
    }

    int cur_num = 1, col_count = 0, row_count = 0, padding = 0;
    for (int i = 0; i <= steps; ++i) {
        const int phase = i % PHASE_COUNT;
        if (phase == 0) {
            const int end_col = col - col_count / 2;
            for (int j = div2_down(col_count); j < end_col; ++j) {
                arr[padding*col + j] = cur_num++; 
            }
            row_count++;
        }
        else if (phase == 1) {
            const int end_row = row - div2_up(row_count);
            for (int j = div2_up(row_count); j < end_row; ++j) {
                arr[j*col + col - padding - 1] = cur_num++;
            }
            col_count++;
        } else if (phase == 2) {
            const int start_col = col - col_count / 2 - 1;
            for (int j = start_col; j >= padding; --j) {
                arr[(row - padding - 1)*col + j] = cur_num++;
            }
            row_count++;
        } else {
            padding++;
            for (int j = row - div2_up(row_count) - 1; j > div2_up(row_count); --j) {
                arr[j*col + padding - 1] = cur_num++;
            }
            col_count++;
        }
    }

    munmap(arr, SIZE);
    return 0;
}