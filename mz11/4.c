#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

void close_pipe(int *fds){
    close(fds[0]);
    close(fds[1]);
}

void kill_pids(int *pids, int count){
    for(int i = 0; i < count; ++i){
        kill(pids[i], SIGKILL);
        waitpid(pids[i], NULL, 0);
    }
}

int main(int argC, char *argV[]) {
    if (argC == 1) {
        return 0;
    } else if (argC == 2) {
        int pid1 = fork();
        if (pid1 == -1) {
            exit(1);
        } else if (!pid1) {
            execlp(argV[1], argV[1], NULL);
            _exit(1);
            
        }
        waitpid(pid1, NULL, 0);
        return 0;
    }
    int *pids = calloc(argC - 1, sizeof(*pids));
    int pfd[2];
    if (pipe(pfd) == -1){
        goto free_pids;
    }
    pids[0] = fork();
    if (pids[0] == -1) {
        goto free_pids;
    } else if (!pids[0]) {
        if (pfd[1] != 1){
            dup2(pfd[1], 1);
        }
        close_pipe(pfd);
        execlp(argV[1], argV[1], NULL);
        goto free_pids;
    }
    int pfd_new[2] = {pfd[0], pfd[1]};
    for(int i = 2; i < argC - 1; ++i){
        if (pipe(pfd_new) == -1) {
            kill_pids(pids, i - 1);
            goto free_pids;
        }
        pids[i - 1] = fork();
        if (pids[i - 1] == -1) {
            kill_pids(pids, i - 1);
            goto free_pids;
        } else if (!pids[i - 1]) {
            if (pfd[0] != 0) {
                dup2(pfd[0], 0);
            }
            if (pfd_new[1] != 1) {
                dup2(pfd_new[1], 1);
            }
            close_pipe(pfd);
            close_pipe(pfd_new);
            execlp(argV[i], argV[i], NULL);
            goto free_pids;
        }
        close_pipe(pfd);
        pfd[0] = pfd_new[0];
        pfd[1] = pfd_new[1];
    }
    pids[argC - 2] = fork();
    if (pids[argC - 2] == -1) {
        kill_pids(pids, argC - 2);
        goto free_pids;
    } else if (!pids[argC - 2]) {
        if (pfd_new[0] != 0) {
            dup2(pfd_new[0], 0);
        }
        close_pipe(pfd_new);
        execlp(argV[argC - 1], argV[argC - 1], NULL);
        goto free_pids;
    }
    close_pipe(pfd_new);
    for(int i = 0; i < argC - 1; ++i){
        waitpid(pids[i], NULL, 0);
    }
    free(pids);
    return 0;

free_pids:
    free(pids);
    exit(1);
}