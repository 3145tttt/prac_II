#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <limits.h>

int
main(int argC, char *argV[])
{
    const int fd = open(argV[1], O_RDONLY);

    uint16_t min = UINT16_MAX;
    unsigned char bites[sizeof(min)];

    while (read(fd, bites, sizeof(min)) == sizeof(min)) {
        const uint16_t cur = (uint16_t) bites[0] << CHAR_BIT | bites[1];
        if((cur & 1) == 0 && cur < min)
            min = cur;
    }

    if (min != UINT16_MAX)
        printf("%"PRIu16"\n", min);
    if(fd != -1)
        close(fd);
    return 0;
}