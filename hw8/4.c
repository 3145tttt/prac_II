#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

typedef struct FuncArgs
{
    unsigned char args[64];
} FuncArgs;

int
main(int argC, char *argV[])
{   
    enum { BASE = 10, SIG_IND = 3 };

    void *handle = dlopen(argV[1], RTLD_LAZY);
    void *f = dlsym(handle, argV[2]);
    FuncArgs my_args;
    const int LEN = strlen(argV[SIG_IND]);
    int offset = 0;
    for (int i = 1; i < LEN; ++i) {
        const char type = argV[SIG_IND][i];
        if (type == 'i'){
            char *end;
            int t = strtol(argV[SIG_IND + i], &end, BASE);
            memcpy(my_args.args + offset, &t, sizeof(t));
            offset += sizeof(t);
        } else if (type == 'd') {
            char *end;
            double t = strtod(argV[SIG_IND + i], &end);
            memcpy(my_args.args + offset, &t, sizeof(t));
            offset += sizeof(t);
        } else {
            char *t = argV[SIG_IND + i];
            memcpy(my_args.args + offset, &t, sizeof(t));
            offset += sizeof(t);
        }
    }

    if (argV[SIG_IND][0] == 'i'){
        printf("%d\n", ((int (*) (FuncArgs)) f)(my_args));
    } else if (argV[SIG_IND][0] == 'd') {
        printf("%.10g\n", ((double (*) (FuncArgs)) f)(my_args));
    } else if (argV[SIG_IND][0] == 's'){
        printf("%s\n", ((char * (*) (FuncArgs)) f)(my_args));
    } else {
        ((void (*) (FuncArgs)) f)(my_args);
    }
    return 0;
}