#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/eventfd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>

void operation(int *data, int ind1, int ind2, int value);

enum { BASE = 10, PADDING = 5 };

void worker(int *arr, int semid, unsigned rand_seed, int iter_count, int count) {
    srand(rand_seed);
    for(int i = 0; i < iter_count; ++i){
        int ind1 = rand() % count;
        int ind2 = rand() % count;
        int value = rand() % BASE;
        if(ind1 != ind2) {
            struct sembuf b1[] =
            {
                { .sem_num = ind1, -1, SEM_UNDO },
                { .sem_num = ind2, -1, SEM_UNDO },
            };
            int r = semop(semid, b1, sizeof(b1) / sizeof(b1[0]));
            if (r < 0) {
                exit(1);
            }
            operation(arr, ind1, ind2, value);
            struct sembuf b2[] =
            {
                { .sem_num = ind1, 1, SEM_UNDO },
                { .sem_num = ind2, 1, SEM_UNDO },
            };
            r = semop(semid, b2, sizeof(b2) / sizeof(b2[0]));
            if (r < 0) {
                exit(1);
            }
        }
    }
}

int main(int argC, char *argV[]) {
    int count = strtol(argV[1], NULL, BASE);
    int key = strtol(argV[2], NULL, BASE);
    int nproc = strtol(argV[3], NULL, BASE);
    int iter_count = strtol(argV[4], NULL, BASE);
    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        fprintf(stderr, "semget: %s\n", strerror(errno));
        exit(1);
    }

    //set all 1
    short *ptr = calloc(count, sizeof(ptr[0]));
    for (int i = 0; i < count; ++i) ptr[i] = 1;
    semctl(semid, 0, SETALL, ptr);
    free(ptr);
    
    //make array
    int *arr = mmap(NULL, count * sizeof(*arr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (arr == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(1);
    }
    for(int i = 0; i < count; ++i){
        scanf("%d", arr + i);
    }

    //start proc
    for(int i = 0; i < nproc; ++i) {
        unsigned seed = strtol(argV[PADDING + i], NULL, BASE);
        int pid = fork();
        if (!pid) {
            worker(arr, semid, seed, iter_count, count);
            exit(0);
        } else if (pid < 0) {
            exit(1);
        }
    }
    
    //wait
    while (wait(NULL) > 0) { }
    for(int i = 0; i < count; ++i){
        printf("%d\n", arr[i]);
    }
    semctl(semid, 0, IPC_RMID);
    munmap(arr, count * sizeof(*arr));
    return 0;
}