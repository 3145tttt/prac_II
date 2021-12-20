#include <stdio.h>
#include <stdlib.h>

enum { MAX = 2 };

void make_2_from_16(char *str, unsigned long long *num){
    for(int i = 0; i < MAX; ++i){
        char split_s[17];
        snprintf(split_s, sizeof(split_s), "%16s", str + 16*i);
        num[i] = strtoull(split_s, NULL, 16);
    }
}

void add(unsigned long long *num1, unsigned long long *num2, unsigned long long *res){
    unsigned carry_flag = __builtin_add_overflow(num1[1], num2[1], &res[1]);
    __builtin_add_overflow(num1[0], num2[0], &res[0]);
    res[0] += carry_flag;
    
}

int main(int argC, char *argV[]){
    unsigned long long num1[MAX];
    unsigned long long num2[MAX];

    make_2_from_16(argV[1], num1);
    make_2_from_16(argV[2], num2);
    //a - b
    unsigned long long for_inc[MAX] = {0, 1};
    num2[0] = ~num2[0];
    num2[1] = ~num2[1];
    add(num2, for_inc, num2);
    //a + (~b + 1)
    unsigned long long res[MAX] = {0};
    add(num1, num2, res);
    for(int i = 0; i < MAX; ++i){
        printf("%016llx", res[i]);
    }
    printf("\n");
    return 0;
}

