#if 0
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include "helpers.h"
#include "procfs.h"
#include "util.h"

struct task_info expected_tasks[] = {
    { 218,   0, "virtlogd", "disk sleep" },
    { 817,   0, "stress",   "running" },
    { 902,   0, "virtlogd", "disk sleep" },
    { 4490,  0, "virtlogd", "stopped" },
    { 7079,  0, "ccp-7-q2", "running" },
    { 11131, 0, "kworker/16:1H", "disk sleep" },
    { 78241, 0, "nfsv4.1-svc",   "tracing stop" },
    { 88888, 0, "really_really_long_named_", "zombie" },
};

int comparator(const void *t1, const void *t2)
{
    struct task_info *info1 = (struct task_info *) t1;
    struct task_info *info2 = (struct task_info *) t2;
    return (long) info1->pid - (long) info2->pid;
}

test_start("Checks the task list display");

subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");

    struct task_stats *tstats = pfs_create_tstats();
    test_assert(tstats != NULL);

    pfs_tasks(fakeproc, tstats);

    int active_task_count = tstats->total - tstats->sleeping;
    test_assert(active_task_count == 8, "There are 8 active tasks in the test dataset");

    qsort(tstats->active_tasks,
            active_task_count,
            sizeof(struct task_info),
            comparator);

    for (int i = 0; i < active_task_count; ++i) {
        printf("\n> -- Task %d --", i);
        test_assert(tstats->active_tasks[i].pid == expected_tasks[i].pid);
        test_printf("%d", tstats->active_tasks[i].pid);
        test_printf("%d", expected_tasks[i].pid);
        test_assert_str(tstats->active_tasks[i].name, "==", expected_tasks[i].name, 500);
        test_printf("%s", tstats->active_tasks[i].name);
        test_assert_str(tstats->active_tasks[i].state, "==", expected_tasks[i].state, 500);
        test_printf("%s", tstats->active_tasks[i].state);
    }

    free(fakeproc);
    pfs_destroy_tstats(tstats);
});

test_end
