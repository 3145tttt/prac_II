#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

unsigned long long max_num;

void
worker(int id, FILE *f1, FILE *f2)
{
    while(1) {
        unsigned long long cur;
        fscanf(f1, "%llu", &cur);
        if(cur >= max_num) {
            fclose(f1);
            fclose(f2);
            return;
        }
        printf("%d %llu\n", id, cur);
        fflush(stdout);
        cur++;
        fprintf(f2, "%llu\n", cur);
        fflush(f2);
    }
}


int
main(int argC, char *argV[])
{
    max_num = strtoll(argV[1], NULL, 10);
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);

    FILE *fds1[2] = { fdopen(p1[0], "r"), fdopen(p1[1], "w") };
    FILE *fds2[2] = { fdopen(p2[0], "r"), fdopen(p2[1], "w") };
    if (!fork()) {
        fclose(fds1[1]);
        fclose(fds2[0]);
        worker(1, fds1[0], fds2[1]);
        _exit(0);
    }
    fclose(fds1[0]);
    fclose(fds2[1]);
    if (!fork()) {
        worker(2, fds2[0], fds1[1]);
        _exit(0);
    }
    fprintf(fds1[1], "%d\n", 1);
    fflush(fds1[1]);
    fclose(fds1[1]);
    fclose(fds2[0]);
    while (wait(NULL) > 0) {}
    printf("Done\n");
    return 0;
}