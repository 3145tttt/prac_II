#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argC, char *argV[])
{
    const char MASK[] = "rwxrwxrwx";
    enum { BASE = 8, PATTERN_SIZE = sizeof(MASK) - 1 };
    char *end;
    for (int i = 1; i < argC; ++i) {
        int x = strtol(argV[i], &end, BASE);
        for (int j = 0; j < PATTERN_SIZE; ++j) {
            int t = x & (1 << (PATTERN_SIZE - 1 - j));
            if (t) {
                putchar(MASK[j]);
            } else {
                putchar('-');
            }
        }
        putchar('\n');
    }
    return 0;
}