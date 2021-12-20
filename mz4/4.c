#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

typedef struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
} Node;

void
dfs(int fd, int32_t key){
    if(key == 0)
        return;
    Node cur;

    if (lseek(fd, key*sizeof(cur), SEEK_SET) == (off_t)-1) {
        fprintf(stderr, "error lseek\n");
        exit(1);
    }
    if (read(fd, &cur, sizeof(cur)) != sizeof(cur)) {
        fprintf(stderr, "error read current node\n");
        exit(1);
    }
    
    dfs(fd, cur.right_idx);
    printf("%d ", cur.key);
    dfs(fd, cur.left_idx);
    return;
}

int
main(int argC, char *argV[]){
    const int fd = open(argV[1], O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "error open\n");
        exit(1);
    }

    Node head;
    if (read(fd, &head, sizeof(head)) != sizeof(head)) {
        fprintf(stderr, "error read head\n");
        exit(1);
    }

    dfs(fd, head.right_idx);
    printf("%d ", head.key);
    dfs(fd, head.left_idx); 
    printf("\n");
    close(fd);
    return 0;
}
