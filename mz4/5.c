#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

int
main(int argC, char *argV[])
{
    int fd = open(argV[1], O_RDWR);
    if(fd == -1){
        fprintf(stderr, "open error\n");
        return 1;
    }

    long long min, offset = -1, x;
    int readed = 0;
    for (int i = 0; (readed = read(fd, &x, sizeof(min))) == sizeof(min); ++i) {
        if (offset == -1) {
            min = x;
            offset = i;
        } if (x < min) {
            min = x;
            offset = i;
        }
    }

    if (readed == -1) {
        fprintf(stderr, "read error\n");
        return 1;
    }

    if (offset == -1)
        return 0;

    if (lseek(fd, offset * sizeof(min), SEEK_SET) == (off_t) -1) {
        fprintf(stderr, "lseek error\n");
        return 1;
    }


    if (min != INT64_MIN)
        min = -min;

    if (write(fd, &min, sizeof(min)) != sizeof(min)) {
        close(fd);
        fprintf(stderr, "write error\n");
        return 1;
    }
    
    close(fd);
    return 0;
}