#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int main(int argC, char *argV[]){
    struct timeval T;
    double lambda;
    int K;
    unsigned int S;

    sscanf(argV[1], "%lld", (long long *) &T.tv_sec);
    sscanf(argV[2], "%ld",  &T.tv_usec);
    sscanf(argV[3], "%lf",  &lambda);
    sscanf(argV[4], "%d",  &K);
    sscanf(argV[5], "%u",  &S);

    srand(S);
    enum { MICRO_SEC = 1000000 };
    for(int i = 0; i < K; ++i){
        const double U = (double) rand() / RAND_MAX;
        const long DIF = -log(U) / lambda;

        T.tv_usec += DIF;
        T.tv_sec += T.tv_usec / MICRO_SEC;
        T.tv_usec %= MICRO_SEC;

        printf("%lld %ld\n", (long long)T.tv_sec, T.tv_usec);
    }
    return 0;
}