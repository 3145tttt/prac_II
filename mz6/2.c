#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int
have_suf_exe(char *s, const char *pattern) //i use strlen(pattern) > 0 and pattern != NULL
{
    const char* pos = strrchr(s, pattern[0]);
    if (!pos)
        return 0;
    int len = strlen(pattern);
    for (int i = 1; i <= len; ++i){
        if (pos[i] != pattern[i]) {
            return 0;
        }
    }
    return 1;
}

int
main(int argC, char *argV[])
{
    unsigned long long count = 0;
    DIR *my_dir = opendir(argV[1]);
    if (my_dir == NULL) {
        fprintf(stderr, "dir open error\n");
        exit(1);
    }
    struct dirent *cur;
    char path[PATH_MAX];

    while ((cur = readdir(my_dir))) {
        struct stat stats;

        if (snprintf(path, sizeof(path), "%s/%s", argV[1], cur->d_name) < sizeof(path) 
                && stat(path, &stats) != -1 && S_ISREG(stats.st_mode) 
                && !access(path, X_OK) && have_suf_exe(cur->d_name, ".exe")) {
            ++count;
        }
    }

    printf("%llu\n", count);
    closedir(my_dir);
    return 0;
}