#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int 
main(int argC, char *argV[])
{
    enum { COUNT = 7, MAX_SIZE = 256};

    const int fd = open(argV[1], O_RDONLY);

    uint16_t count_per_block, sum_prev, arr[MAX_SIZE];
    int hash[COUNT] = {0}, sum = 0;
    long long ans[COUNT] = {0};

    while (read(fd, &count_per_block, sizeof(count_per_block)) > 0) {
        read(fd, &sum_prev, sizeof(sum_prev));
        read(fd, arr, count_per_block*sizeof(arr[0]));

        if (sum != sum_prev) {
            for (int i = COUNT - 1; i > 2; --i) {
                ans[i] += hash[i - 1];
                sum_prev -= i*hash[i - 1];
                sum -= (i - 1)*hash[i -1];
            }
            int x = (sum_prev - sum);
            ans[1] += hash[1] - x;
            ans[2] += x;
        } else {
            for (int i = 1; i < COUNT; ++i)
                ans[i] += hash[i];
        }

        for (int i = 0; i < COUNT; ++i)
            hash[i] = 0;
        sum = 0;

        for (uint16_t i = 0; i < count_per_block; ++i) {
            sum += arr[i];
            ++hash[arr[i]];
        }
        
    }

    for (int i = 1; i < COUNT; ++i)
        printf("%lld ", ans[i]);
    
    return 0;
}