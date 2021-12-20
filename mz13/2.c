#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

//return res or 0 if overflow
int64_t check_overflow_mul(uint64_t val1, uint64_t val2){
    uint64_t res = val1 * val2;
    if (val1 > INT64_MAX/val2) {
        return 0;
    }
    return res;
}

//return res or 0 if overflow
int64_t check_overflow_add(uint64_t val1, uint64_t val2){
    uint64_t res = val1 + val2;
    if ((int64_t) res < 0) {
        return 0;
    }
    return res;
}

int64_t calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count) {
    enum { MAX_SHIFT = 64 }; 
    int64_t val1 = check_overflow_mul(8, block_num_size);
    int64_t size_1 = 0;
    if (val1 != 0 && val1 < MAX_SHIFT){
        const int64_t val2 = 1ULL << val1;
        size_1 = check_overflow_mul(val2, block_size);
    }

    const int64_t COUNT_PER_BLOCK = block_size / block_num_size;
    int64_t size_2 = 0;
    val1 = inode_direct_block_count;
    int64_t count_power = 1;
    for(int i = 0; i < 3; ++i){
        count_power = check_overflow_mul(count_power, COUNT_PER_BLOCK);
        if (count_power == 0) {
            goto end;
        }
        val1 = check_overflow_add(val1, count_power);
        if (val1 == 0) {
            goto end;
        }
    }
    size_2 = check_overflow_mul(val1, block_size);
end:
    if(!size_2 && size_1) {
        return size_1;
    } else if (size_2 && !size_1 ){
        return size_2;
    } else if (!size_2 && !size_1){
        return -1;
    } else {
        return size_1 < size_2 ? size_1 : size_2;
    }
}

int main(){
    int64_t block_size, block_num, inode_direct;
    scanf("%"SCNd64"%"SCNd64"%"SCNd64"", &block_size, &block_num, &inode_direct);
    printf("%"PRId64"\n", calc_max_size(block_size, block_num, inode_direct));

    int64_t max = 0, pos = -1;
    for (int64_t i = 1; i < block_size; ++i) {
        int64_t val = calc_max_size(block_size, i, inode_direct);
        if (val == -1){
            printf("%"PRId64" -1\n", i);
            return 0;
        }

        if (max < val) {
            max = val;
            pos = i;
        }
    }
    printf("%"PRId64" %"PRId64"\n", pos, max);
    return 0;
}