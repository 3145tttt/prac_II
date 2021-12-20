#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>

void
my_exec(const char *str){
    FILE *in = fopen(str, "r");
    char path[PATH_MAX + 1];
    if (fgets(path, sizeof(path), in) == NULL) {
        exit(1);
    }
    fclose(in);
    int size = strlen(path);
    path[size - 1] = '\0';
    execl(path, path, NULL);
    exit(1);
}

int
get_status(int pid) {
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

int main(int argC, char *argV[]){
    enum { BASE = 10, PADDING = 2 };

    int N = strtol(argV[1], NULL, BASE);
    const int PROC_COUNT = argC - PADDING;
    int *pids = calloc(PROC_COUNT, sizeof(int));

    for (int i = 0; i < N; ++i) {
        if (i < PROC_COUNT) {
            pids[i] = fork();
            if(!pids[i]) {
                my_exec(argV[PADDING + i]);
            }
        }
    }

    int count = 0;
    for(int i = 0; i < PROC_COUNT && i < N; ++i){
        if(pids[i] != 0 && pids[i] != -1){
            count += get_status(pids[i]);
        }
    }

    for (int i = N; i < PROC_COUNT; ++i) {
        pids[i] = fork();
        if(pids[i] > 0){
            count += get_status(pids[i]);
        } else if(!pids[i]) {
            my_exec(argV[PADDING + i]);
        }
    }

    printf("%d\n", count);
    fflush(stdout);
    return 0;
}