#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    scanf("%d", &n);
    int a[10001];
    int arr[10001];

    int i;
    for(i = 0; i < n; ++i)
        scanf("%d", &a[i]);

    int k, m, r;
    scanf("%d %d %d", &k, &m, &r);
    --k;
    --m;
    int len = m - k + 1;
    r %= len;
    for(i = 0; i < len; ++i){
        arr[(i + r) % len] = a[i + k];
    }

    for(i = 0; i < len; ++i)
        a[k + i] = arr[i];

    for(i = 0; i < n; ++i)
        printf("%d ", a[i]);
    return 0;
}