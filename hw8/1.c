#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


int
main(int argC, char *argV[])
{
    enum {BIAS = 9, BASE = 16, SIZE = 65536, MASK = 0x1FF };

    char *end;
    int offset = strtol(argV[2], &end, BASE);
    int fd = open(argV[1], O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "open error\n");
        exit(1);
    }
    struct stat my_stats;
    if (fstat(fd, &my_stats) == -1) {
        fprintf(stderr, "fstat error\n");
        exit(1);
    }

    if (my_stats.st_size != SIZE) {
        fprintf(stderr, "file size != %d\n", SIZE);
        exit(1);
    }

    short *arr = (short*) mmap(NULL, SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    if (arr == MAP_FAILED) {
        fprintf(stderr, "mmap error\n");
        exit(1);
    }

    unsigned virt_adress;
    while (scanf("%x", &virt_adress) == 1) {
        int virt_page_num = (virt_adress >> BIAS);
        uint16_t hard_page_num = arr[offset/2 + virt_page_num];
        uint16_t hard_address = (virt_adress & MASK) | hard_page_num;
        uint16_t value = arr[hard_address/2];
        printf("%"PRIu16"\n", value);
    }
    munmap(arr, SIZE);
    return 0;
}