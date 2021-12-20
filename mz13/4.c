#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argC, char *argV[]){
    double left = strtod(argV[1], NULL), right = strtod(argV[2], NULL);
    int n = strtol(argV[3], NULL, 10);
    int fd = open("prog.c", O_WRONLY | O_TRUNC | O_CREAT, 0700);

    char start_str[] = "\
#include <stdio.h>\n\
#include <math.h>\n\
double f(double x){\n\
return ";
    
    dprintf(fd, "%s %s; }", start_str, argV[4]);
    close(fd);

    int pid = fork();

    if (!pid) {
        execlp("gcc", "gcc", "prog.c", "-fPIC", "-lm", "-c", NULL);
        fprintf(stderr, "execlp error\n");
        exit(1);
    }
    wait(NULL);
    pid = fork();

    if (!pid) {
        execlp("gcc", "gcc", "prog.o", "-shared", "-o", "prog.so", NULL);
        fprintf(stderr, "execlp error\n");
        exit(1);
    }
    wait(NULL);
    void *handle = dlopen("./prog.so", RTLD_LAZY);
    void *f = dlsym(handle, "f");

    double s = 0, dx = (right - left) / n;

    for(int i = 0; i < n; ++i){
        double x = left + dx * i;
        s += ((double (*) (double)) f)(x) * dx;
    }
    printf("%.10g\n", s);

    remove("./prog.c");
    remove("./prog.o");
    remove("./prog.so");
    return 0;
}

// gcc prog.c -fPIC -lm -c
// gcc prog.o -shared -o prog.so