#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main()
{
    for(int i = 1; i < 3; ++i){
        int pid = fork();
        if (pid > 0){
            wait(NULL);
            if(i == 1){
                printf("%d\n", i);
            } else {
                printf("%d ", i);
            }
            exit(0);
        }
    }
    printf("3 ");
    return 0;
}