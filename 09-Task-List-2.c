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
    { 218,   1041, "virtlogd", 'D' },
    { 817,   1000, "stress",   'R' },
    { 902,   1041, "virtlogd", 'D' },
    { 4490,  1404, "virtlogd", 'T' },
    { 7079,  0, "ccp-7-q2", 'R' },
    { 11131, 0, "kworker/16:1H", 'D' },
    { 78241, 0, "nfsv4.1-svc",   'T' },
    { 88888, 0, "really_really_long_named_", 'Z' },
};

int comparator(const void *t1, const void *t2)
{
    struct task_info *info1 = (struct task_info *) t1;
    struct task_info *info2 = (struct task_info *) t2;

    if (info1->state == 'S' || info1->state == 'I') {
        info1->pid = 10000000;
    }

    if (info2->state == 'S' || info2->state == 'I') {
        info2->pid = 10000000;
    }

    return (long) info1->pid - (long) info2->pid;
}

test_start("Checks the task list display");

subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");

    struct task_stats *tstats = pfs_create_tstats();
    test_assert(tstats != NULL);

    pfs_tasks(fakeproc, NULL, tstats, NULL, NULL);

    int active_task_count = tstats->total - tstats->sleeping;
    test_assert(active_task_count == 8, "There are 8 active tasks in the test dataset");

    qsort(tstats->tasks,
            tstats->total,
            sizeof(struct task_info),
            comparator);

    for (int i = 0; i < 8; ++i) {
        printf("\n> -- Task %d --", i);
        test_assert(tstats->tasks[i].pid == expected_tasks[i].pid);
        test_printf("%d", tstats->tasks[i].pid);
        test_printf("%d", expected_tasks[i].pid);

        test_assert(tstats->tasks[i].uid == expected_tasks[i].uid);
        test_printf("%d", tstats->tasks[i].uid);
        test_printf("%d\n", expected_tasks[i].uid);

        test_assert_str(tstats->tasks[i].name, "==", expected_tasks[i].name, 500);
        test_printf("'%s'", tstats->tasks[i].name);

        test_assert(tstats->tasks[i].state == expected_tasks[i].state);
        test_printf("%c", tstats->tasks[i].state);
        test_printf("%c", expected_tasks[i].state);
    }

    printf("\n\nChecking the next 8 idle / sleeping tasks...\n");
    for (int i = 8; i < 16; ++i) {
        printf("\n> -- Task %d --", i);
        test_assert(tstats->tasks[i].state == 'S' || tstats->tasks[i].state == 'I');
        test_printf("%c", tstats->tasks[i].state);
    }
    
    free(fakeproc);
    pfs_destroy_tstats(tstats);
});


test_end
