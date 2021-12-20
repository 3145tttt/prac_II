#include <limits.h>

STYPE
bit_reverse(STYPE value)
{
    enum { COUNTS = sizeof(STYPE) * CHAR_BIT };
    UTYPE answer = 0;
    for (UTYPE i = 0; i < COUNTS; ++i){
        UTYPE k = value & ((UTYPE) 1 << i) ? 1 : 0;
        answer |= k << (COUNTS - 1 - i);
    }
    return answer;
}