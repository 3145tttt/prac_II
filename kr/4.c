#include <stdio.h>

int main(){
    unsigned arr[256];
    unsigned max = 0;
    int len = 0;
    while (scanf("%u", &arr[len]) == 1) {
        if(max < arr[len]){
            max = arr[len];
        }
        ++len;
    }
    for (int i = len - 1; i >= 0; --i){
        if(arr[i] != 0 && max % arr[i] == 0) {
            printf("%u\n", arr[i]);
        }
    }
    return 0;
}