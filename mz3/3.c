#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum { PRECISION = 10000, PERCENT = 100, START = 2 };

int
main(int argC, char *argV[]) 
{
    char* end;
    double val = strtod(argV[1], &end);
    for (int i = START; i < argC; ++i) {
        double x = strtod(argV[i], &end);
        val *= (1 + x/PERCENT);
        val = round(val * PRECISION) / PRECISION;
    }
    printf("%0.4lf\n", val);
    return 0;
}