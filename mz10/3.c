#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>


int
create_proc(const char *str)
{
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "error fork 1\n");
        return 0;
    } else if (pid == 0) {
        execlp(str, str, NULL);
        _exit(1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
}

int
main(int argC, char *argV[])
{
    if (argC >= 4){
        return !((create_proc(argV[1]) || create_proc(argV[2])) && create_proc(argV[3]));
    }
    return 1;
}