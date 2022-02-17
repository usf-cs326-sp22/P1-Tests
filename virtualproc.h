#ifndef _VIRTUALPROC_H_
#define _VIRTUALPROC_H_

#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debug.h"

#define PATH_SZ 4096
#define MAX_FAKES 1024
#define O_RDONLY 00000000

/* Function prototypes */
int open(const char *pathname, int flags);
int openat(int fd, const char *pathname, int flags);
unsigned int sleep(unsigned int seconds);

/* Function pointers */
int (*orig_open)(const char *pathname, int flags) = NULL;
int (*orig_openat)(int fd, const char *pathname, int flags) = NULL;

/* Globals */
bool enabled = false;

char *stat_loc;
char *mem_loc;
char *load_loc;

int stat_fakes = 0;
int mem_fakes = 0;
int load_fakes = 0;

int last_stat = 0;
int last_mem = 0;
int last_load = 0;

static int num_fakes(const char *location, const char *prefix)
{
    if (location == NULL) {
        return 0;
    }

    int fd = open(location, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 0;
    }

    char path[PATH_SZ];
    int num;
    for (num = 1; num < MAX_FAKES; ++num) {
        sprintf(path, "%s/%s.%d", location, prefix, num);
        fd = open(path, O_RDONLY);
        if (fd == -1) {
            num--;
            break;
        }
        close(fd);
    }

    return num;
}

static void fake_path(char *new_path, size_t path_sz,
        const char *location, const char *prefix,
        int fake_count, int *last_fake)
{
    if (*last_fake >= fake_count) {
        *last_fake = 0;
    }
    (*last_fake)++;

    snprintf(new_path, path_sz, "%s/%s.%d", location, prefix, *last_fake);
}

static bool endswith(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len >= suffix_len) {
        if (strcmp(str + (str_len - suffix_len), suffix) == 0) {
            return true;
        }
    }

    return false;
}

void vp_initialize(void)
{
    orig_open = (int (*)(const char *, int)) dlsym(RTLD_NEXT, "open");
    LOG("Overriding original open() system call at %p -> %p\n",
            orig_open, open);

    orig_openat = (int (*)(int, const char *, int)) dlsym(RTLD_NEXT, "openat");
    LOG("Overriding original openat() system call at %p -> %p\n",
            orig_openat, openat);

    stat_loc = getenv("FAKE_STAT");
    mem_loc = getenv("FAKE_MEMINFO");
    load_loc = getenv("FAKE_LOADAVG");

    stat_fakes = num_fakes(stat_loc, "stat");
    mem_fakes = num_fakes(mem_loc, "meminfo");
    load_fakes = num_fakes(load_loc, "loadavg");

    LOG("Fakes found: stat=%d mem=%d loadavg=%d\n",
            stat_fakes, mem_fakes, load_fakes);
}

void vp_on(void)
{
    enabled = true;
}

void vp_off(void)
{
    enabled = false;
}

int open(const char *pathname, int flags)
{
    //LOG("open(%s)\n", pathname);
    if (enabled == false || pathname == NULL) {
        return (*orig_open)(pathname, flags);
    }

    if (stat_fakes > 0 && endswith(pathname, "stat")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, stat_loc, "stat", stat_fakes, &last_stat);
        //LOG("Replacing open(/proc/stat) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    if (mem_fakes > 0 && endswith(pathname, "meminfo")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, mem_loc, "meminfo", mem_fakes, &last_mem);
        //LOG("Replacing open(/proc/meminfo) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    if (load_fakes > 0 && endswith(pathname, "loadavg")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, load_loc, "loadavg",
                load_fakes, &last_load);
        //LOG("Replacing open(/proc/loadavg) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    return (*orig_open)(pathname, flags);
}

int openat(int fd, const char *pathname, int flags)
{
    //LOG("open(%s)\n", pathname);
    if (enabled == false || pathname == NULL) {
        LOG("Calling orig %d , %s\n", fd, pathname);
        return (*orig_openat)(fd, pathname, flags);
    }

    if (stat_fakes > 0 && endswith(pathname, "stat")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, stat_loc, "stat", stat_fakes, &last_stat);
        LOG("Replacing open(/proc/stat) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    if (mem_fakes > 0 && endswith(pathname, "meminfo")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, mem_loc, "meminfo", mem_fakes, &last_mem);
        LOG("Replacing open(/proc/meminfo) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    if (load_fakes > 0 && endswith(pathname, "loadavg")) {
        char new_path[PATH_SZ];
        fake_path(new_path, PATH_SZ, load_loc, "loadavg",
                load_fakes, &last_load);
        LOG("Replacing open(/proc/loadavg) with open(%s)\n", new_path);
        return (*orig_open)(new_path, flags);
    }

    return (*orig_open)(pathname, flags);
}


unsigned int sleep(unsigned int seconds)
{
    return 0;
}

#endif
