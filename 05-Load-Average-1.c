#if 0
    export FAKE_LOADAVG="${TEST_DIR}/inputs/virtualproc/loadavg"
    source "${TEST_LIB}/crunner" -linspector -D_GNU_SOURCE -ldl -lm
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <math.h>

#include "helpers.h"
#include "procfs.h"
#include "virtualproc.h"

test_start("Tests the pfs_load_avg function.");

subtest("Predefined load averages, simulated by dynamically generating "
        "/proc/loadavg contents for each call to open(2). "
        "See tests/inputs/virtualproc/loadavg for the sequence of file states.",
{
    struct load_avg expected_loads[] = {
            { 0.99, 0.99, 0.99 },
            { 78.10, 40.40, 40.40 },
            { 99.99, 99.99, 99.99 },
            { 1.00, 1.00, 1.11 },
    };

    vp_initialize();
    vp_on();
    for (int i = 0; i < 4; ++i) {
        printf("\n\n> --- Test Iteration: %d ---\n", i);
        struct load_avg load = pfs_load_avg("/proc");
        struct load_avg expected_load = expected_loads[i];

        test_assert(load.one == expected_load.one);
        test_printf("%.2f", load.one);
        test_printf("%.2f", expected_load.one);

        test_assert(load.five == expected_load.five);
        test_printf("%.2f", load.five);
        test_printf("%.2f", expected_load.five);

        test_assert(load.fifteen == expected_load.fifteen);
        test_printf("%.2f", load.fifteen);
        test_printf("%.2f", expected_load.fifteen);
    }

});

subtest("Using fakeproc dataset",
{
    vp_off();

    char *fakeproc = build_path(getenv("TEST_DIR"), "/inputs/fakeproc");
    struct load_avg load = pfs_load_avg(fakeproc);
    free(fakeproc);

    test_assert(load.one == 1.87);
    test_printf("%.2f", load.one);

    test_assert(load.five == 1.27);
    test_printf("%.2f", load.five);

    test_assert(load.fifteen == 1.07);
    test_printf("%.2f", load.fifteen);
});

test_end
