#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

volatile sig_atomic_t flag;
void handler(int s)
{
    flag = 1;
}

int main(int argC, char *argV[])
{
    const int MAX = strtol(argV[1], NULL, 10);
    int v = 1;
    sigset_t s1, s2;
    sigemptyset(&s1); sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, NULL);
    sigemptyset(&s2);
    int fd[2];
    pipe(fd);
    FILE *fd_files[2] = { fdopen(fd[0], "r"), fdopen(fd[1], "w") };
    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    int pid = fork();
    int pid1;
    int id = 1;
    if (!pid) {
        pid = getpid();
    } else {
        pid1 = pid;
        pid = fork();
        if (!pid) {
            pid = getpid();
            id = 2;
        } else {
            fprintf(fd_files[1], "%d %d\n", v, pid);
            fflush(fd_files[1]);
            kill(pid1, SIGUSR1);
            fclose(fd_files[0]);
            fclose(fd_files[1]);
            while (wait(NULL) > 0) {}
            printf("Done\n");
            return 0;
        }
    }
    while (1) {
        while (!flag) {
            sigsuspend(&s2);
        }
        flag = 0;
        int son_pid;
        fscanf(fd_files[0], "%d%d", &v, &son_pid);
        if (v != MAX) {
            printf("%d %d\n", id, v);
            fflush(stdout);
            ++v;
        }
        fprintf(fd_files[1], "%d %d\n", v, pid);
        fflush(fd_files[1]);
        kill(son_pid, SIGUSR1);
        if ( v == MAX) {
            break;
        }
    }
    fclose(fd_files[0]);
    fclose(fd_files[1]);
    return 0;
}