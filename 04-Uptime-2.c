#if 0
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "helpers.h"
#include "procfs.h"

test_start("Tests the pfs_uptime and pfs_format_uptime functions.");

subtest("Invalid proc location. Return value should be nonzero.",
{
    char invalid[] = "/this/does/not/exist";
    double time = pfs_uptime(invalid);

    test_assert(time == 0.0 || time == -1.0);
    test_printf("%f", time);
});

subtest("Retrieving the uptime in seconds from dummyproc1",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);
    test_assert((uptime == 1489135.87));
    test_printf("%f", uptime);
});

subtest("Retrieving the formatted uptime from dummyproc1",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc1");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);

    char formatted_uptime[1024];
    int return_value = pfs_format_uptime(uptime, formatted_uptime);
    test_assert(return_value == 0);
    test_printf("%d", return_value);

    if (return_value == 0) {
        test_assert_str(
                formatted_uptime,
                "==",
                "17 days, 05:38:55",
                1024);
        test_printf("'%s'", formatted_uptime);
    }
});

subtest("Retrieving the uptime in seconds from dummyproc3",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc3");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);
    test_assert((uptime == 13373.57));
    test_printf("%f", uptime);
});

subtest("Retrieving the formatted uptime from dummyproc3",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc3");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);

    char formatted_uptime[1024];
    int return_value = pfs_format_uptime(uptime, formatted_uptime);
    test_assert(return_value == 0);
    test_printf("%d", return_value);

    if (return_value == 0) {
        test_assert_str(
                formatted_uptime,
                "==",
                "03:42:53",
                1024);
        test_printf("'%s'", formatted_uptime);
    }
});

subtest("Retrieving the uptime in seconds from dummyproc4",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc4");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);
    test_assert((uptime == 86400.00));
    test_printf("%f", uptime);
});

subtest("Retrieving the formatted uptime from dummyproc4",
{
    char *dummyproc = build_path(getenv("TEST_DIR"), "/inputs/dummyproc4");
    double uptime = pfs_uptime(dummyproc);
    free(dummyproc);

    char formatted_uptime[1024];
    int return_value = pfs_format_uptime(uptime, formatted_uptime);
    test_assert(return_value == 0);
    test_printf("%d", return_value);

    if (return_value == 0) {
        test_assert_str(
                formatted_uptime,
                "==",
                "1 day, 00:00:00",
                1024);
        test_printf("'%s'", formatted_uptime);
    }
});

test_end
