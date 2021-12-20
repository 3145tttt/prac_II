#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argC, char *argV[]){

    unsigned long long size = 0;
    enum { MASK = 0x3FF };
    for(int i = 1; i < argC; ++i){
        struct stat buf;
        const int ERROR = lstat(argV[i], &buf);
        if(!ERROR && ((buf.st_size & MASK) == 0) 
            && buf.st_nlink == 1 && S_ISREG(buf.st_mode))
            size += buf.st_size;
    }

    printf("%llu\n", size);
    return 0;
}