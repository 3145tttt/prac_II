#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main()
{
    enum { BUF_SIZE = 9, PROC_NUM = 3, BASE = 10 };
    char num_str[BUF_SIZE] = {0};
    
    for(int i = 0; i < PROC_NUM; ++i){
        int pid = fork();
        if(pid == 0){
            read(0, num_str, sizeof(num_str) - 1);
            char *end;
            int x = strtol(num_str, &end, BASE);
            printf("%d %d\n", i + 1, x*x);
            fflush(stdout);
            exit(0);
        }
    }
    while (wait(NULL) > 0) {}
    return 0;
}
