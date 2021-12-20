#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void
go_back(const char *s, int *x)
{
    int i = *x;
    while(i > 0 && s[i] != '/'){
        --i;
    }
    *x = i;
}

int
make_clean_path(const char *s, char *clean_path, int N)
{
    int len = 0;
    for(int i = 0; i < N; ++i){
        if(i + 2 <= N && s[i] == '/' && s[i + 1] == '.' && (s[i + 2] == '/' || s[i + 2] == '\0')){
            i += 1;
        } else if(i + 3 <= N && s[i] == '/' && s[i + 1] == '.' && s[i + 2] == '.' 
                && (s[i + 3] == '/' || s[i + 3] == '\0')){
            --len;
            if(len < 0)
                len = 0;
            go_back(s, &len);
            i += 2;
        } else {
            clean_path[len] = s[i];
            ++len;
        }
    }
    if(len == 0)
        clean_path[len++] = '/';
    clean_path[len] = '\0';
    return len;
}

int
prefix_len(const char *s1, const char *s2, int n1, int n2)
{
    const int N = n1 < n2 ? n1 : n2;
    int prefix_len = 0;
    for(int i = 1; i < N; ++i){
        if(s1[i] != s2[i])
            break;
        if(s1[i] == '/')
            ++prefix_len;
    }
    return prefix_len;
}

int
path_depth(const char *s, int n)
{
    int len = 0;
    for(int i = 1; i < n; ++i){
        if(s[i] == '/')
            ++len;
    }
    return len;
}

void
constract_anwer(const char *path, int n, int count_back, int count_pref, char *answer, int flag)
{
    if(flag){
        if(count_back == 1){
            answer[0] = '.';
            answer[1] = '\0';  
            return;
        }

        --count_back;
        int len = 0;
        for(int i = 0; i < count_back; ++i){
            answer[len++] = '.';
            answer[len++] = '.';
            answer[len++] = '/';
        }
        --len;
        answer[len] = '\0';
        return;
    }
    if(count_back == 0 && count_pref == 0 && n == 1){
        answer[0] = '.';
        answer[1] = '\0';  
        return;
    }
        
    int len = 0;
    for(int i = 0; i < count_back; ++i){
        answer[len++] = '.';
        answer[len++] = '.';
        answer[len++] = '/';
    }

    for(int i = 1; i < n; ++i){
        if(count_pref <= 0){
            answer[len++] = path[i];
        } else if(path[i] == '/'){
            --count_pref;
        }
    }
    if(n == 1 && path[0] == '/'){
        --len;
    }
    answer[len] = '\0';
}

int
file_in(const char *path1, const char *path2, int n1, int n2)
{
    if(n1 < n2)
        return 0;
    for(int i = 0; i < n2; ++i){
        if(path1[i] != path2[i])
            return 0;
    }
    if(path1[n2] == '/')
        return 1;
    return 0;
}

char
*relativize_path(const char *path1, const char *path2)
{
    const int N1 = strlen(path1);
    const int N2 = strlen(path2);
    enum { PAD = 2};
    char *path1_clean = calloc(N1 + PAD, sizeof(char));
    char *path2_clean = calloc(N2 + PAD, sizeof(char));
    const int n1 = make_clean_path(path1, path1_clean, N1);
    const int n2 = make_clean_path(path2, path2_clean, N2);

    int pref_len = prefix_len(path1_clean, path2_clean, n1, n2);
    int count_back = path_depth(path1_clean, n1) - pref_len;
    int f = file_in(path1_clean, path2_clean, n1, n2);

    char *answer = calloc(PAD*(n1 + n2), sizeof(char));
    constract_anwer(path2_clean, n2, count_back, pref_len, answer, f);
    
    free(path1_clean);
    free(path2_clean);
    return answer;
}