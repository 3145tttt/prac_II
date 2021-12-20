#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>

int main(int argC, char *argV[]){

    char filename[NAME_MAX];
    int fd = open("/dev/urandom", O_RDONLY);
    unsigned long long random_bytes;
    read(fd, &random_bytes, sizeof(random_bytes));
    int pid = getpid();
    snprintf(filename, sizeof(filename), "script_%d_%llu", pid, random_bytes);

    char my_dir[PATH_MAX];
    char *cur_dir = getenv("XDG_RUNTIME_DIR");
    if (cur_dir == NULL) {
        cur_dir = getenv("TMPDIR");
    }
    if (cur_dir == NULL) {
        cur_dir = "/tmp";
    }
    snprintf(my_dir, PATH_MAX, "%s/%s", cur_dir, filename);

    FILE *prog_file = fopen(my_dir, "w");
    const char start_str[] = "#! /usr/bin/python3 \nimport os\nprint(";
    fprintf(prog_file, "%s", start_str);
    for(int i = 1; i < argC - 1; ++i){
        fprintf(prog_file, "%s*", argV[i]);
    }
    fprintf(prog_file, "%s", argV[argC - 1]);
    const char end_str[] = ")\nos.remove(__file__)";
    fprintf(prog_file, "%s", end_str);
    fclose(prog_file);
    chmod(my_dir, 0500);
    execlp(my_dir, filename, NULL);
}