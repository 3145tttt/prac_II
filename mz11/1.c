#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    enum { YEAR = 1900 };

    int fds[2];
    pipe(fds);
    int pid = fork();
    if (pid > 0) { //father
        close(fds[1]);
        wait(NULL);
        time_t cur_time;
        read(fds[0], &cur_time, sizeof(cur_time));
        close(fds[0]);
        struct tm cur_dates;
        localtime_r(&cur_time, &cur_dates);
        printf("Y:%4d\n", cur_dates.tm_year + YEAR);
        fflush(stdout);
    } else if (!pid){ //son
        pid = fork();
        if (pid > 0) {
            close(fds[1]);
            wait(NULL);
            time_t cur_time;
            read(fds[0], &cur_time, sizeof(cur_time));
            close(fds[0]);
            struct tm cur_dates;
            localtime_r(&cur_time, &cur_dates);
            printf("M:%2d\n", cur_dates.tm_mon + 1);
            fflush(stdout);
        } else if (!pid) { //sonson
            pid = fork();
            if (pid > 0) {
                close(fds[1]);
                wait(NULL);
                time_t cur_time;
                read(fds[0], &cur_time, sizeof(cur_time));
                close(fds[0]);
                struct tm cur_dates;
                localtime_r(&cur_time, &cur_dates);
                printf("D:%2d\n", cur_dates.tm_mday);
                fflush(stdout);
            } else if (!pid) { //sonsonson
                close(fds[0]);
                time_t cur_time = time(NULL);
                write(fds[1], &cur_time, sizeof(cur_time));
                write(fds[1], &cur_time, sizeof(cur_time));
                write(fds[1], &cur_time, sizeof(cur_time));
                close(fds[1]);
            } else {
                exit(1);
            }
        } else {
            exit(1);
        }
    } else {
        exit(1);
    }
    return 0;
}