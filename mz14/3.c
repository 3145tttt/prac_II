#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/eventfd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <inttypes.h>
#include <errno.h>

struct Msgbuffer
{
    int type;
    int64_t value1;
    int64_t value2;
};

void worker(int num, int n, int id, int64_t maxval) {
    while (1){
        struct Msgbuffer t;
        int l = sizeof(t) - sizeof(t.type);
        // printf("wait worker = %d %d\n", num, n);
        int r = msgrcv(id, &t, l, num, 0);
        if (r == -1) {
            if (errno == EIDRM) {
                exit(0);
            }
            fprintf(stderr, "get error %d %s\n", num, strerror(errno));
            _exit(1);
        }
        if (t.value2 > maxval) {
            msgctl(id, IPC_RMID, NULL);
            _exit(0);
        }
        int64_t res = t.value1 + t.value2;
        printf("%d %"PRIu64"\n", num - 1, res);
        fflush(stdout);

        int next = res % n;
        t.type = next + 1;
        t.value1 = t.value2;
        t.value2 = res;

        r = msgsnd(id, &t, l, 0);
        if (r == -1) {
            if (errno == EIDRM) {
                exit(0);
            }
            _exit(1);
        }
    }
    
}


int main(int argC, char *argV[]){
    enum { BASE = 10 };
    int key = strtol(argV[1], NULL, BASE);
    int n = strtol(argV[2], NULL, BASE);
    int64_t val1 = strtoll(argV[3], NULL, BASE);
    int64_t val2 = strtoll(argV[4], NULL, BASE);
    int64_t max_val = strtoll(argV[5], NULL, BASE);

    int msgid = msgget(key, IPC_CREAT | 0660);
    if (msgid < 0) {
        printf("msg error\n");
        exit(1);
    }

    for (int i = 0; i < n; ++i){
        int pid = fork();
        if (!pid) {
            worker(i + 1, n, msgid, max_val);
            exit(0);
        } else if(pid == -1) {
            msgctl(msgid, IPC_RMID, NULL);
            while ( wait(NULL) > 0){}
            return 1;
        }
    }

    struct Msgbuffer t;
    int l = sizeof(t) - sizeof(t.type);
    t.type = 1;
    t.value1 = val1;
    t.value2 = val2;
    int r = msgsnd(msgid, &t, l, 0);
    if (r == -1){
        exit(1);
    }

    while ( wait(NULL) > 0){}
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}