#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>


int main(int argC, char *argV[]){

    enum { BASE = 10 };
    int fd = open(argV[1], O_RDWR);
    if (fd == -1){
        exit(1);
    }

    int my_fd = open("my_file_for_prog", O_TRUNC | O_RDWR | O_CREAT, 0660);
    if (my_fd == -1){
        exit(1);
    }
    const unsigned long long max = strtoull(argV[2], NULL, BASE);
    unsigned long long x;
    int count = 0;
    while (read(fd, &x, sizeof(x)) == sizeof(x)) {
        if(x >= max) {
            if (write(my_fd, &x, sizeof(x)) != sizeof(x)) {
                exit(1);
            }
            ++count;
        }
    }
    lseek(my_fd, 0, SEEK_SET);
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, sizeof(x)*count);
    
    while (read(my_fd, &x, sizeof(x)) == sizeof(x)) {
        if (write(fd, &x, sizeof(x)) != sizeof(x)) {
            exit(1);
        }
    }
    close(fd);
    close(my_fd);
    remove("my_file_for_prog");
    
    return 0;
}