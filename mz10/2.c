#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
//code from  github cmc-os
int mysys(const char *str) {
    int pid = fork();
    if (pid < 0) {
        return -1;
    } else if (!pid) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    } else {
        return WEXITSTATUS(status);
    }
}