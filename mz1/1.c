#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int v, t;
    scanf("%d\n %d", &v, &t);
    if(v > 0)
        printf("%d", v*t % 109);
    else
        printf("%d", (109 - (abs(v)*t % 109)) %109);
    return 0;
}