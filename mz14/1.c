#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/mman.h>

int pids[401];
int nproc;

void handler_end(int s) {
    for(int i = 1; i <= nproc; ++i){
        kill(pids[i], SIGINT);
    }
}


void work(int num, int *arr, int *prev, int max, int nproc, int *fds) {
    while(1) {
        uint64_t t;
        read(fds[num], &t, sizeof(t));

        int val = arr[0];
        printf("%d %d %d\n", num, val, prev[0]);
        fflush(stdout);
        ++val;
        if (val > max) {
            kill(getppid(), SIGUSR1);
            _exit(0);
        }
        unsigned long long x = val % nproc;
        x = (x * x) % nproc;
        x = (x * x) % nproc;
        int new_id = x + 1;
        arr[0] = val;
        prev[0] = num;

        t = 1;
        write(fds[new_id], &t, sizeof(t));
    }
}

int main(int argC, char *argV[]){
    nproc = strtol(argV[1], NULL, 10);
    int max_val = strtol(argV[3], NULL, 10);

    int *arr = mmap(NULL, sizeof(*arr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    int *prev = mmap(NULL, sizeof(*prev), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    int fds[401];
    for(int i = 1; i <= nproc; ++i){
        fds[i] = eventfd(0, EFD_SEMAPHORE);
    }
    for(int i = 1; i <= nproc; ++i){
        int pid = fork();
        if(!pid) {
            work(i, arr, prev, max_val, nproc, fds);
            exit(0);
        } else if (pid < 0){
            exit(0);
        }
        pids[i] = pid;
    }

    sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler_end, .sa_flags = SA_RESTART }, NULL);
    uint64_t t = 1;
    arr[0] = 0;
    prev[0] = 0;
    write(fds[1], &t, sizeof(t));
    while (wait(NULL) > 0){}
    for(int i = 1; i <= nproc; ++i){
        close(fds[i]);
    }
    munmap(arr, sizeof(*arr));
    munmap(prev, sizeof(*prev));
    return 0;
}