#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argC, char *argV[]){

    int pid = fork();
    if (pid < -1) {
        fprintf(stderr, "fork error\n");
        exit(1);
    } else if(pid == 0){
        //code from cmc-os github
        int fd = open(argV[2], O_RDONLY | O_CLOEXEC);
        if (fd < 0) {
            _exit(42);
        }
        if(dup2(fd, 0) == -1) {
            _exit(42);
        }

        fd = open(argV[3], O_WRONLY | O_CREAT | O_CLOEXEC | O_APPEND, 0660);
        if (fd < 0) {
            _exit(42);
        }
        if(dup2(fd, 1) == -1) {
            _exit(42);
        }

        fd = open(argV[4], O_WRONLY | O_CREAT | O_CLOEXEC | O_TRUNC, 0660);
        if (fd < 0) {
            _exit(42);
        }
        if(dup2(fd, 2) == -1) {
            _exit(42);
        }

        execlp(argV[1], argV[1], NULL);
        _exit(42);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        printf("%d", status);
    }
    return 0;
}