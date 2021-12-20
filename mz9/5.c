#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main()
{   
    int parrent_pid = getpid();
    int pid = 0;
    if (pid == 0){
        while(1){
            int x;
            int status;
            if(pid == 0){
                int fields = scanf("%d ", &x);
                if (fields != 1) {
                    exit(0);
                }
            } else if (pid > 0) {
                wait(&status);
                if (!WIFEXITED(status)) {
                    exit(1);
                }
                if (!WEXITSTATUS(status)){
                    printf("%d\n", x);
                }

                if (getpid() == parrent_pid) {
                    exit(0);
                }
                exit(WEXITSTATUS(status));
            } else if (pid == -1) {
                printf("-1\n");
                exit(1);
            }
            pid = fork();
        }
    }
    wait(NULL);
    return 0;
}