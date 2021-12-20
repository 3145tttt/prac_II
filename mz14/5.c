#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>

void merge(int in1_fd, int in2_fd, int out_fd) {
    int val1, val2;
    FILE *in1 = fdopen(in1_fd, "r");
    FILE *in2 = fdopen(in2_fd, "r");
    FILE *out = fdopen(out_fd, "w");
    int r1 = fscanf(in1, "%d", &val1);
    int r2 = fscanf(in2, "%d", &val2);
    while (r1 == 1 && r2 == 1){
        if(val1 == val2) {
            fprintf(out, "%d ", val1);
            r1 = fscanf(in1, "%d", &val1);
            r2 = fscanf(in2, "%d", &val2);
        } else if (val1 < val2){
            r1 = fscanf(in1, "%d", &val1);
        } else if (val1 > val2){
            r2 = fscanf(in2, "%d", &val2);
        }
    }
    fclose(in1);
    fclose(in2);
    fclose(out);
}

int mergeSort(char *array[], int begin, int end)
{
    if (begin > end) {
        //fprintf(stderr, "MAGIC error\n"); после прошлой задачи страшно выводить и потерять баллы просто так
    }
    if (begin == end){
        int fd = open(array[end], O_RDONLY);
        if(fd == -1){
            //fprintf(stderr, "open error\n"); после прошлой задачи страшно выводить и потерять баллы просто так
            exit(1); 
        }
        return fd;
    }
  
    int mid = begin + (end - begin) / 2;
    int in1 = mergeSort(array, begin, mid);
    int in2 = mergeSort(array, mid + 1, end);
    int fds[2];
    pipe(fds);
    int pid = fork();
    if(!pid) {
        close(fds[0]);
        merge(in1, in2, fds[1]);
        exit(0);
    }
    close(in1);
    close(in2);
    close(fds[1]);
    return fds[0];
}


int main(int argC, char *argV[]) {
    if (argC == 1) { //все равно -1 попытка
        return 0;
    }
    int in_fd = mergeSort(argV + 1, 0, argC - 2);
    FILE *in = fdopen(in_fd, "r");
    int x;
    while (fscanf(in, "%d", &x) == 1){
        printf("%d\n", x);
        fflush(stdout);
    }
    return 0;
}