#include <stdio.h>

int
parse_rwx_permissions(const char *str)
{
    if (str == NULL)
        return -1;
    static const char PATTERN[] = "rwxrwxrwx";
    enum { SIZE = sizeof(PATTERN) - 1 };
    int answer = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (str[i] == '\0')
            return -1;
        if (str[i] == PATTERN[i]) {
            answer += 1 << (SIZE - 1 - i);
        } else if (str[i] != '-')
            return -1;
    }
    if (str[SIZE] != '\0')
        return -1;
    return answer;
}