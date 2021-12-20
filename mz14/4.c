#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>

void worker(int num, int count, int semid) {
    setbuf(stdin, NULL);
    while (1) {
        struct sembuf b1[] = {
            { .sem_num = num, -1, SEM_UNDO }
        };
        int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
        if (r < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            exit(1);
        }
        int x;
        //fflush(stdin);
        if (scanf("%d", &x) != 1) {
            semctl(semid, 0, IPC_RMID);
            exit(0);
        }
        //fflush(stdin);
        printf("%d %d\n", num, x);
        fflush(stdout);
        int next = ((x % count) + count) % count;
        
        struct sembuf b2[] = {
            { .sem_num = next, 1, SEM_UNDO }
        };
        r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
        if (r < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            exit(1);
        }
    }
}

int main(int argC, char *argV[]) {
    enum { BASE = 10 };
    int count = strtol(argV[1], NULL, BASE);
    const int key = 42;
    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        exit(1);
    }

    //start proc
    for(int i = 0; i < count; ++i) {
        int pid = fork();
        if (!pid) {
            worker(i, count, semid);
            exit(0);
        } else if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            exit(1);
        }
    }
    
    struct sembuf b[] = {
        { .sem_num = 0, 1, SEM_UNDO }
    };
    int r = semop(semid, b, sizeof(b) / sizeof(b[0]));
    if (r < 0) {
        if (errno == EIDRM) {
            exit(0);
        }
        exit(1);
    }
    //wait
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    return 0;
}