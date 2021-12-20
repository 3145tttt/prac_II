#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);
    int a[101];
    int arr1[101], arr2[101];
    int i = 0, l1 = 0, l2 = 0;


    for(i = 0; i < n; ++i)
        scanf("%d", &a[i]);

    for(i = 0; i < n; ++i){
        if(a[i] & 1){
            arr1[l1] = a[i];
            ++l1;
        } else {
            arr2[l2] = a[i];
            ++l2;
        }
    }

    int j = 0;
    for(i = 0; i < l1 - 1; ++i)
        for(j = i; j < l1; ++j){
            if(arr1[i] > arr1[j]){
                int x = arr1[i];
                arr1[i] = arr1[j];
                arr1[j] = x;
            }
        }

    for(i = 0; i < l2 - 1; ++i)
        for(j = i; j < l2; ++j){
            if(arr2[i] > arr2[j]){
                int x = arr2[i];
                arr2[i] = arr2[j];
                arr2[j] = x;
            }
        }

    for(i = 0; i < l2; ++i)
        printf("%d ", arr2[i]);

    for(i = 0; i < l1; ++i)
        printf("%d ", arr1[i]);

    return 0;
}