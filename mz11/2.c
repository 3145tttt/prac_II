#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    signal(SIGPIPE, SIG_IGN);
    int fds[2];
    pipe(fds);

    int pid = fork();
    if (pid > 0) {
        int x;
        close(fds[0]);
        while(scanf("%d ", &x) != EOF) {
            write(fds[1], &x, sizeof(x));
        }
        close(fds[1]);
        wait(NULL);
    } else if (!pid) {
        pid = fork();
        if (pid > 0) {
            close(fds[0]);
            close(fds[1]);
            wait(NULL);
        } else if (!pid) {
            long long s = 0;
            int x;
            close(fds[1]);
            while(1){
                if (read(fds[0], &x, sizeof(x))) {
                    s += x;
                } else {
                    break;
                }
            }
            close(fds[0]);
            printf("%lld\n", s);
        }
    }

    return 0;
}