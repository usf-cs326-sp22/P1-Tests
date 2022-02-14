#if 0
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include "helpers.h"
#include "procfs.h"

test_start("Checks the task state counters are tallied accurately");

subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");

    struct task_stats *tstats = pfs_create_tstats();
    test_assert(tstats != NULL);

    pfs_tasks(fakeproc, tstats);

    test_assert(tstats->running == 2);
    test_printf("%zu", tstats->running);

    test_assert(tstats->waiting == 3);
    test_printf("%zu", tstats->waiting);

    test_assert(tstats->sleeping == 1479);
    test_printf("%zu", tstats->sleeping);

    test_assert(tstats->stopped == 2);
    test_printf("%zu", tstats->stopped);

    test_assert(tstats->zombie == 1);
    test_printf("%zu", tstats->zombie);

    free(fakeproc);
    pfs_destroy_tstats(tstats);
});

subtest("Comparing against the tiny 'dummyproc' test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1");

    struct task_stats *tstats = pfs_create_tstats();
    test_assert(tstats != NULL);

    pfs_tasks(fakeproc, tstats);

    test_assert(tstats->running == 0);
    test_printf("%zu", tstats->running);

    test_assert(tstats->waiting == 1);
    test_printf("%zu", tstats->waiting);

    test_assert(tstats->sleeping == 5);
    test_printf("%zu", tstats->sleeping);

    test_assert(tstats->stopped == 0);
    test_printf("%zu", tstats->stopped);

    test_assert(tstats->zombie == 0);
    test_printf("%zu", tstats->zombie);

    free(fakeproc);
    pfs_destroy_tstats(tstats);
});

test_end
