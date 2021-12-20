#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int N;
    scanf("%d", &N);
    int pid = 0;
    for(int i = 1; i <= N; ++i) {
        if(pid == 0){
            printf("%d", i);
            if(i == N){
                printf("\n");
                fflush(stdout);
                exit(0);
            }
            printf(" ");
            fflush(stdout);
            pid = fork();
            if (pid > 0) {
                wait(NULL);
                exit(0);
            }
        }
    }
    return 0;
}