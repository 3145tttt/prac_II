#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


enum { ADD = 0, MUL = 1 };

volatile int state = ADD;

void handler1(int s) {
    state = ADD;
}

void handler2(int s) {
    state = MUL;
}

int main() {
    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction){ .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int tmp;
    unsigned s = 0;
    while(scanf("%d", &tmp) == 1){
        if (state == ADD) {
            s += tmp;
        } else {
            s *= tmp;
        }
        printf("%d\n", s);
        fflush(stdout);
    }
    return 0;
}