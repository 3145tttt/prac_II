#include <string.h>
#include <stdio.h>
#include <limits.h>


enum { BASE = 10 };

int correct_start(char c){
    return c == '+' || c == '-' || (c >= '0' && c <= '9');
}

long long convert_str(char *s, int n, int *error){
    if(!correct_start(s[0])){
        *error = -1;
        return 0;
    }
    int sign = 1;
    int start_pos = 0;
    if(s[0] == '-'){
        sign = -1;
        ++start_pos;
    }

    if(s[0] == '+')
        ++start_pos;

    int end_pos = 0;
    for(int i = start_pos; i < n; ++i)
        if(s[i] < '0' || s[i] > '9'){
            end_pos = i;
            break;
        }

    if(s[end_pos] == 'k' && !(end_pos + 2 == n && (s[end_pos + 1] == '+' || s[end_pos + 1] == '-'))){
        *error = -1;
        return 0;
    }

    if(s[end_pos] != 'k' && !(end_pos + 1 == n && (s[end_pos] == '+' || s[end_pos] == '-'))){
        *error = -1;
        return 0;
    }
    
    if(s[end_pos] == '\0'){
        *error = -1;
        return 0;
    }

    int multiply = 1, opp_code = 1;
    if(s[end_pos] == 'k'){
        multiply = 1000;
        if(s[end_pos + 1] == '-'){
            opp_code = -1;
        }
    } else {
        if(s[end_pos] == '-'){
            opp_code = -1;
        }
    }

    long long number = 0;
    for(int i = start_pos; i < end_pos; ++i){
        number *= BASE;
        number += (s[i] - '0');
        if(number > INT_MAX){
            *error = -1;
            return 0;
        }
        if(number < INT_MIN){
            *error = -1;
            return 0;
        }
    }
    number = number * multiply * sign;
    if(number > INT_MAX){
        *error = -1;
        return 0;
    }
    if(number < INT_MIN){
        *error = -1;
        return 0;
    }
    return number*opp_code;
}


int main(int argC, char *argV[]){
    long long answer = 0;
    for(int i = 1; i < argC; ++i){
        int error_flag = 0;
        long long x = convert_str(argV[i], strlen(argV[i]), &error_flag);
        if(!error_flag)
            answer += x;
    }
    printf("%lld\n", answer);
    return 0;
}