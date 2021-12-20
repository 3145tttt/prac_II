#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

enum { BYTE_INIT = 7 };
//for send
volatile sig_atomic_t send = 0;
char out_byte;
volatile int pos_in_byte = BYTE_INIT;

//SIGUSR1 SIGUSR2
void handler_get(int s){
    send = 0;
    if(s == SIGUSR2) {
        out_byte |= 1 << pos_in_byte;
    }
    --pos_in_byte;
}
//SIGALRM
void handler_send(int s){
    send = 1;
}
//SIGIO
void handler_end(){
    exit(0);
}

int pid_son1;
void send_byte(unsigned char c) {
    for(int pos = 7; pos >= 0; --pos){
        int t = (c >> pos) & 1;
        //start send
        send = 0;
        if(t){
            kill(pid_son1, SIGUSR2);
        } else {
            kill(pid_son1, SIGUSR1);
        }
        while(!send){
            pause();
        }
    }
}

void get_byte() {
    while(send) {
        pause();
    }
    send = 1;
    if(pos_in_byte == -1){
        printf("%c", out_byte);
        fflush(stdout);
        out_byte = 0;
        pos_in_byte = BYTE_INIT;
    }
    kill(0, SIGALRM);
}


int main(int argC, char *argV[]){
    sigaction(SIGALRM, &(struct sigaction){ .sa_handler = handler_send, .sa_flags = SA_RESTART }, NULL);    
    pid_son1 = fork();

    if (!pid_son1) {
        signal(SIGALRM, SIG_IGN);
        sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler_get, .sa_flags = SA_RESTART }, NULL);
        sigaction(SIGUSR2, &(struct sigaction){ .sa_handler = handler_get, .sa_flags = SA_RESTART }, NULL);
        sigaction(SIGIO, &(struct sigaction){ .sa_handler = handler_end, .sa_flags = SA_RESTART }, NULL);

        send = 1;
        kill(0, SIGALRM);
        while (1){
            get_byte();
        }
        exit(0);
    } else if (pid_son1 < 0) {
        exit(1);
    }

    int pid_son2 = fork();
    if (!pid_son2) {
        while(!send) {
            pause();
        }
        int fd = open(argV[1], O_RDONLY);
        unsigned char cur;
        while(read(fd, &cur, sizeof(cur)) == sizeof(cur)){
            send_byte(cur);
        }
        kill(pid_son1, SIGIO);
        close(fd);
        exit(0);
    } else if (pid_son2 < 0) {
        exit(1);
    }
    signal(SIGALRM, SIG_IGN);
    while(wait(NULL) > 0) {};
    return 0;
}