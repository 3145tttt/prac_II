#include <stdio.h>
#include <stdlib.h>

enum { BASE = 10 };

int
main(int argC, char *argV[])
{
    long long sum_pos = 0, sum_neg = 0;
    char *end;
    for (int i = 1; i < argC; ++i) {
        const int CUR = strtol(argV[i], &end, BASE);
        if (CUR > 0) {
            sum_pos += CUR;
        } else {
            sum_neg += CUR;
        }
    }
    printf("%lld\n%lld\n", sum_pos, sum_neg);
    return 0;
}