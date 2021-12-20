#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

enum 
{
    BIAS_4 = 20,
    BIAS_3 = 12,
    BIAS_2 = 8,
    MASK_FIRST = 0x0F,
    MASK_SECOND = 0xFF
};

int main(int argC, char *argV[]){
    const int fd = open(argV[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "open error\n");
        return 1;
    }
    unsigned int x;
    while(scanf("%u", &x) > 0) {
        unsigned char ans[sizeof(x)] = {(x >> BIAS_4) & MASK_FIRST, (x >> BIAS_3) 
                & MASK_SECOND, (x >> BIAS_2) & MASK_FIRST, x & MASK_SECOND};
        if (write(fd, ans, sizeof(x)) != sizeof(x)) {
            close(fd);
            fprintf(stderr, "write error\n");
            return 1;
        }
    }
    close(fd);
    return 0;
}