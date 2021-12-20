#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argC, char *argV[]){

    enum { FILE1 = 4, FILE2 = 5, CMD1 = 1, CMD2 = 2, CMD3 = 3 };
    int pfd[2];
    pipe(pfd);
    pid_t pid1 = fork();
    pid_t pid_cmd2 = -1;
    if (!pid1) {
        dup2(pfd[1], 1);
        close(pfd[1]);
        close(pfd[0]);
        pid_t pid_cmd1 = fork();
        if(!pid_cmd1){
            int fd = open(argV[FILE1], O_RDONLY);
            if (fd == -1){
                exit(1);
            }
            dup2(fd, 0);
            close(fd);
            execlp(argV[CMD1], argV[CMD1], NULL);
            _exit(1);
        } else if(pid_cmd1 == -1){
            exit(1);
        }
        int status;
        waitpid(pid_cmd1, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            pid_cmd2 = fork();
            if(!pid_cmd2){
                execlp(argV[CMD2], argV[CMD2], NULL);
                _exit(1);
            } else if(pid_cmd2 == -1){
                exit(1);
            }
            waitpid(pid_cmd2, &status, 0);
        }
        return 0;
    } else if(pid1 == -1) {
        exit(1);
    }
    close(pfd[1]);
    pid_t pid2 = fork();
    if (!pid2) {
        int fd = open(argV[FILE2], O_WRONLY | O_APPEND | O_CREAT, 0600);
        if (fd == -1){
            exit(1);
        }
        dup2(fd, 1);
        dup2(pfd[0], 0);
        close(pfd[0]);
        close(fd);
        fflush(stdout);
        execlp(argV[CMD3], argV[CMD3], NULL);
        _exit(1);
    } else if(pid2 == -1){
        exit(1);
    }
    close(pfd[0]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}