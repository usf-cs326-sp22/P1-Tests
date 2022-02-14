#if 0
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include "helpers.h"
#include "util.h"

test_start("Percentage bar display. "
"Hint: when drawing the bar, remember to round the percentage.");

static const double NaN = 0.0 / 0.0;
static const double pos_inf = 1.0 / 0.0;
static const double neg_inf = -1.0 / 0.0;

subtest("Percentage inputs that should yield an empty 0% percentage bar",
{
    char *expected_bar = "[--------------------] 0.0%";
    char bar[30];

    double input;

    input = 0.0;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = -0.4;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = -0.0;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = NaN;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = neg_inf;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);
});


subtest("Percentage inputs that should yield a 100% percentage bar",
{
    char *expected_bar = "[####################] 100.0%";
    char bar[30];

    double input;

    input = 1.0;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = 3.8;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = 100.0;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);

    input = pos_inf;
    display_percbar(bar, input);
    test_assert_str(bar, "==", expected_bar, 30);
    test_printf("%f", input);
    test_printf("%s\n", bar);
});

subtest("A variety of percentage inputs",
{
    double inputs[] = {
        0.01, 0.044, 0.045, 0.05, 0.1, 0.2, 0.3, 0.35, 0.4, 0.44, 0.45, 0.5,
        0.74, 0.899999, 0.91524, 0.98, 0.99, 0.994, 0.995 };

    char *expected_outputs[] = {
			"[--------------------] 1.0%",
			"[--------------------] 4.4%",
			"[#-------------------] 4.5%",
			"[#-------------------] 5.0%",
			"[##------------------] 10.0%",
			"[####----------------] 20.0%",
			"[######--------------] 30.0%",
			"[#######-------------] 35.0%",
			"[########------------] 40.0%",
			"[########------------] 44.0%",
			"[#########-----------] 45.0%",
			"[##########----------] 50.0%",
			"[##############------] 74.0%",
			"[##################--] 90.0%",
			"[##################--] 91.5%",
			"[###################-] 98.0%",
			"[###################-] 99.0%",
			"[###################-] 99.4%",
			"[####################] 99.5%",
	};

    char bar[30];

    for (int i = 0; i < sizeof(inputs) / sizeof(double); ++i) {
        display_percbar(bar, inputs[i]);
        test_assert_str(bar, "==", expected_outputs[i], 30);
        test_printf("%f", inputs[i]);
        test_printf("%s\n", bar);
    }
});

test_end
