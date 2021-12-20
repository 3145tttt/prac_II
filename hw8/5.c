#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

int
my_strcmp(const void *a, const void *b)
{
    const char **a_ = (const char **)a;
    const char **b_ = (const char **)b;
    return strcasecmp(*a_, *b_);
}

void
dfs(char *path)
{
    DIR *dp = opendir(path);
    if (dp == NULL)
        return;
    struct dirent *cur_dp;

    enum { START_LEN = 10, MULT = 2 };
    int len = START_LEN;
    int ind = 0;
    char **my_vector = (char **)malloc(len*sizeof(char *));

    if (my_vector == NULL) {
        fprintf(stderr, "Allocate error\n");
        exit(1);
    }

    while ((cur_dp = readdir(dp))) {
        if (len == ind) {
            len *= MULT;
            my_vector = (char **) realloc(my_vector, len*sizeof(*my_vector));
            if (my_vector == NULL) {
                fprintf(stderr, "Allocate error\n");
                exit(1);
            }
        }
        char full_path[PATH_MAX];
        if (strcmp(cur_dp->d_name, ".") != 0 && strcmp(cur_dp->d_name, "..") != 0 
            && snprintf(full_path, sizeof(full_path), "%s/%s", path, cur_dp->d_name) < sizeof(full_path)) {
            struct stat my_stat;
            if (lstat(full_path, &my_stat) == -1) {
                fprintf(stderr, "lstat error\n");
                exit(1);
            }
            
            if (S_ISDIR(my_stat.st_mode)) {
                char *name = strdup(cur_dp->d_name);
                if (name == NULL) {
                    fprintf(stderr, "Allocate error\n");
                    exit(1);
                }
                my_vector[ind++] = name;
            }
        }
    }
    free(cur_dp);

    qsort(my_vector, ind, sizeof(*my_vector), my_strcmp);
    char *full_path = (char *) malloc(PATH_MAX);
    if (full_path == NULL) {
        fprintf(stderr, "Allocate error\n");
        exit(1);
    }

    closedir(dp);
    for (int i = 0; i < ind; ++i) {
        snprintf(full_path, PATH_MAX, "%s/%s", path, my_vector[i]);
        DIR *dp_cur = opendir(full_path);
        if (dp_cur != NULL) {
            printf("cd %s\n", my_vector[i]);
            free(my_vector[i]);
            closedir(dp_cur);
            dfs(full_path);
            printf("cd ..\n");
        } else {
            free(my_vector[i]);
        }
    }
    free(full_path);
    free(my_vector);
    return;
}

int
main(int argC, char *argV[])
{
    dfs(argV[1]);
    return 0;
}