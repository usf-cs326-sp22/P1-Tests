#if 0
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "helpers.h"
#include "procfs.h"

test_start(
        "Verifies that the pfs_kernel_version function retrieves the kernel "
        "version with correct formatting.");

subtest("Comparing output against uname(2)",
{
    struct utsname ubuf;
    int uname_ret = uname(&ubuf);
    test_assert(uname_ret == 0);
    char *expected_release = strtok(ubuf.release, "-");
    
    char release[64] = { 88 };
    int return_value = pfs_kernel_version("/proc", release, 64);

    test_assert_str(release, "==", expected_release, 64);
    test_printf("'%s'", release);

    test_assert(return_value == 0);
    test_printf("%d", return_value);
});


subtest("Comparing against fake procfs test set",
{
    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");
    char release[64] = { 88 };
    int return_value = pfs_kernel_version(fakeproc, release, 64);
    free(fakeproc);

    test_assert_str(release, "==", "1.33.7", 64);
    test_printf("'%s'", release);

    test_assert(return_value == 0);
    test_printf("%d", return_value);
});


subtest("Invalid proc location. Return value should be nonzero.",
{
    char invalid[4096] = "/this/does/not/exist";
    char release[64] = { 88 };
    int return_value = pfs_kernel_version(invalid, release, 64);

    test_assert(return_value != 0);
    test_printf("%d", return_value);
});

test_end
