#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/stat.h>

int main(int argC, char *argV[]) {
    //from 10-4
    char filename[NAME_MAX];
    char progname[NAME_MAX];
    int fd = open("/dev/urandom", O_RDONLY);
    unsigned long long random_bytes;
    read(fd, &random_bytes, sizeof(random_bytes));
    close(fd);
    int pid = getpid();
    snprintf(progname, sizeof(progname), "prog_%d_%llu", pid, random_bytes);
    snprintf(filename, sizeof(filename), "prog_%d_%llu.c", pid, random_bytes);

    char *cur_dir = getenv("XDG_RUNTIME_DIR");
    if (cur_dir == NULL) {
        cur_dir = getenv("TMPDIR");
    }
    if (cur_dir == NULL) {
        cur_dir = "/tmp";
    }
    char my_dir[PATH_MAX];
    char full_prog[PATH_MAX];
    snprintf(my_dir, sizeof(my_dir), "%s/%s", cur_dir, filename);
    snprintf(full_prog, sizeof(full_prog), "%s/%s", cur_dir, progname);

    //make prog code
    FILE *prog_file = fopen(my_dir, "w");
    const char start_str[] = "#include <stdlib.h>\n#include<stdio.h>\nint main(int argC, char *argV[]){\n\
int reject = 0, summon = 1, disqualify = 2;\nint x = strtol(argV[1], NULL, 10);\nint res = ";

    const char end_str[] = "\nchar strs[][11] = {\"reject\", \"summon\", \"disqualify\"};\n\
printf(\"%s\\n\", strs[res]);\nreturn 0;\n}";

    fprintf(prog_file, "%s %s;%s", start_str, argV[1], end_str);
    fclose(prog_file);
    chmod(my_dir, 0500);

    //main program
    pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Fork error\n");
        exit(1);
    } else if(!pid) {
        execlp("gcc", "gcc", my_dir, "-o", full_prog, NULL);
        fprintf(stderr, "execlp gcc error\n");
        exit(1);
    }
    wait(NULL);
    char x[10];
    while (scanf("%10s", x) != EOF) {
        pid = fork();
        if(pid < 0) {
            fprintf(stderr, "Fork error\n");
            exit(1);
        } else if(!pid) {
            execl(full_prog, progname, x, NULL);
            fprintf(stderr, "execlp prog error\n");
            exit(1);
        }
        wait(NULL);
    }

    remove(my_dir);
    remove(full_prog);
    return 0;
}
