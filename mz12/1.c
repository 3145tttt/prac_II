#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int s)
{
    enum { MAX = 5 };
    static int count = 0;
    if (count == MAX) {
        exit(0);
    }
    printf("%d\n", count);
    fflush(stdout);
    ++count;
}

int main()
{
    sigaction(SIGHUP, &(struct sigaction){ .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    
    return 0;
}