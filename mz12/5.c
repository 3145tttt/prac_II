#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <math.h>
#include <unistd.h>

int
is_prime(int number)
{
    if (number == 1 || (number & 1) == 0) {
        return 0;
    }
    int lim = sqrt(number) + 1;
    for (int i = 3; i < lim; i += 2) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

volatile sig_atomic_t printed = 0;

void
handler1(int s)
{
    ++printed;
}

void
handler2(int s)
{
    exit(0);
}

volatile int cur = 0;

void
print_val(){
    enum { MAX = 3 };
    static int count = 0;
    if (count == MAX) {
        exit(0);
    }
    printf("%d\n", cur);
    fflush(stdout);
    ++count;
}

int
main(int argC, char *argV[]){

    int low, high;
    scanf("%d%d", &low, &high);

    sigaction(SIGINT, &(struct sigaction){ .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction){ .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);

    for (int i = low; i < high; ++i){
        if (is_prime(i)) {
            cur = i;
        }
        while (printed) {
            print_val();
            --printed;
        }
    }
    printf("-1\n");
    fflush(stdout);
    return 0;
}