#ifndef _HELPERS_H_
#define _HELPERS_H_

char *build_path(char *proc_dir, char *suffix)
{
    size_t len = strlen(proc_dir) + strlen(suffix);
    char *str = malloc(sizeof(char) * len + 1);
    strcpy(str, proc_dir);
    strcat(str, suffix);
    return str;
}

char *unreadable(char *suffix)
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1/");
    char *file = build_path(dummyproc, suffix);
    int chmod_ret = chmod(file, 0000);
    test_assert(chmod_ret == 0, "If this fails the test dataset is broken");
    free(file);
    return dummyproc;
}

char *sys_exec(char *command)
{

    return NULL;
}

#endif
