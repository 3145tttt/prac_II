#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argC, char *argV[]){
    enum { BASE = 10, CASH_ARG = 2, BLOCK_ARG = 3, STR_ARGS = 4, DEF_VAL = -1}; 
    int cache_size = strtol(argV[CASH_ARG], NULL, BASE);
    int block_size = strtol(argV[BLOCK_ARG], NULL, BASE);
    const int BLOCK_COUNTS = cache_size / block_size;

    int *cache_emulator = (int *)calloc(BLOCK_COUNTS, sizeof(*cache_emulator));
    for(int i = 0; i < BLOCK_COUNTS; ++i){
        cache_emulator[i] = DEF_VAL;
    }
    char op_str[3];
    int addr, size, value, answer = 0;
    while (scanf("%3s%x%d%d", op_str, &addr, &size, &value) == STR_ARGS) {
        const int BLOCK_ORD = addr / block_size;
        const int IND = BLOCK_ORD % BLOCK_COUNTS;
        answer += (op_str[0] == 'R' && cache_emulator[IND] != BLOCK_ORD && cache_emulator[IND] != DEF_VAL);
        cache_emulator[IND] = BLOCK_ORD; 
    }
    free(cache_emulator);
    printf("%d\n", answer);
    return 0;
}