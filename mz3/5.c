#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int
main()
{
    enum { BUF_SIZE = 100000, NUM_COUNT = 10, STR_END_2 = 2};
    char *buffer = malloc(BUF_SIZE), filename[PATH_MAX];
    unsigned long long hash[10] = {0};

    if (fgets(filename, sizeof(filename), stdin) == NULL)
        goto error;

    int len = strlen(filename);

    if (len > 1 && filename[len - STR_END_2] == '\r')
        filename[len - STR_END_2] = '\0';
    else if (len > 1 && filename[len - 1] == '\n')
        filename[len - 1] = '\0';

    if (filename[0] == '\0')
        goto error;

    const int fd_1 = open(filename, O_RDONLY);
    
    if (fd_1 == -1)
        goto error;

    int cur_size;
    while ((cur_size = read(fd_1, buffer, BUF_SIZE)) > 0)
        for (int i = 0; i < cur_size; ++i) {
            const int CUR = buffer[i] - '0';
            if (CUR > -1 && CUR < NUM_COUNT)
                ++hash[CUR];
        }

    close(fd_1);
error:
    for (int i = 0; i < NUM_COUNT; ++i)
        printf("%d %llu\n", i, hash[i]);

    free(buffer);
    return 0;
}