#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    OWNER_MASK = 0700,
    GROUP_MASK = 070,
    OTHER_MASK = 07,
    OWNER_BIAS = 6,
    GROUP_BIAS = 3
};

static int
check(int mode, int access, int bias, int mask)
{
    return (((mode & mask) >> bias) & access) == access;
}

static int
check_group(unsigned *gids, unsigned cur_gid, int n, int access, int mode)
{
    for (int i = 0; i < n; ++i)
        if (cur_gid == gids[i])
            return check(mode, access, GROUP_BIAS, GROUP_MASK);
    return -1;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if(!task->uid)
        return 1;

    const int MODE = stb->st_mode, N = task->gid_count; 
    if (task->uid == stb->st_uid)
        return check(MODE, access, OWNER_BIAS, OWNER_MASK);
    
    const int res_group = check_group(task->gids, stb->st_gid, N, access, MODE);
    if (res_group != -1)
        return res_group;
    
    return check(MODE, access, 0, OTHER_MASK);
}
