#include <stdio.h>
#include <inttypes.h>

int main(){
    uint32_t n, s;
    int w;
    fscanf(stdin, "%"SCNu32"%"SCNu32"%d", &n, &s, &w);
    const uint32_t MAX_N = ~0u >> (32 - n), MASK = MAX_N >> 1, SF = 1u << (n - 1);
    uint32_t num = 0;

    while(num <= MAX_N - s){
        int32_t neg = (num & SF) == 0 ? num : -(num & MASK);
        printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", w, num, w, num, w, neg);
        num += s;
    }
    int32_t neg = (num & SF) == 0 ? num : -(num & MASK);
    printf("|%*"PRIo32"|%*"PRIu32"|%*"PRId32"|\n", w, num, w, num, w, neg);
    return 0;
}