#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

enum 
{
    BIAS = 8,
    MASK = 0xFF,
    INFO_SIZE = 20,
    NAME_INFO = 16,
    GALLEON_PER_S = 17,
    SICKLE_PER_K = 29
};

int
convert(unsigned char x[])
{
    int ans = 0;
    for (int i = 0; i < sizeof(ans); ++i) {
        int offset = BIAS * (sizeof(ans) - 1 - i);
        ans |= ((unsigned) x[NAME_INFO + i] << offset) & ((unsigned) MASK << offset);
    }
    return ans;
}

int
main(int argC, char *argV[])
{
    int find_1 = 0, find_2 = 0;
    long long max_1 = 0, max_2 = 0;
    unsigned char buf[INFO_SIZE];

    for (int i = 1; i < argC; ++i) {
        const int fd = open(argV[i], O_RDONLY);
        while (read(fd, buf, sizeof(buf)) == sizeof(buf)) {
            const long long CUR = convert(buf);
            if (!find_1 || CUR > max_1) {
                max_2 = max_1;
                max_1 = CUR;
                if(find_1)
                    find_2 = 1;
                find_1 = 1;
            } else if (CUR != max_1 && (!find_2 || CUR > max_2)) {
                max_2 = CUR;
                find_2 = 1;
            }
        }
        if (fd != -1)
            close(fd);
    }

    if (find_2 == 0)
        return 0;

    if (max_2 < 0) {
        printf("-");
        max_2 = -max_2;
    }

    long long g = max_2 / (GALLEON_PER_S * SICKLE_PER_K);
    long long s = (max_2 / SICKLE_PER_K) % GALLEON_PER_S;
    long long k = (max_2 % SICKLE_PER_K);

    printf("%lldg%02llds%02lldk\n", g, s, k);
    return 0;
}