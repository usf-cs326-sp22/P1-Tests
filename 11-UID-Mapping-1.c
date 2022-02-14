#if 0
    awk -F':' '{print $3}' /etc/passwd | sort -n > "${TEST_DIR}/inputs/all_uids"
    source "${TEST_LIB}/crunner" -linspector
#endif

#include <limits.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"
#include "procfs.h"
#include "util.h"

test_start("Tests uid_to_uname function.");

subtest("Comparing output against known username (root)",
{
    char name[16] = { 'X' };
    uid_to_uname(name, 0);
    test_assert_str(name, "==", "root", 16);
    test_printf("'%s'", name);
});

subtest("Mapping an unknown UID should yield the same UID in string form",
{
    char *uid_file = build_path(getenv("TEST_DIR"), "/inputs/all_uids");

    /* First, we need to find a UID that is *NOT* valid on this system. Luckily
     * we have sorted the list of UIDs so this is a bit easier... */

    FILE *fp = fopen(uid_file, "r");
    test_assert(fp != NULL);

    int tested = 0;
    uid_t uid = 0;
    char line[128];
    while (fgets(line, 128, fp) && tested < 3) {
        if (atoi(line) != uid) {
            printf("Using UID: %d\n", uid);

            char name[16] = { 'X' };
            uid_to_uname(name, uid);

            char same[16];
            snprintf(same, 16, "%d", uid);

            test_assert_str(name, "==", same, 16);
            test_printf("'%s'", name);

            tested++;
        }

        uid++;
    }
    fclose(fp);

});

subtest("Comparing output against getpwuid(3). Also tests username length truncation.",
{
    char *uid_file = build_path(getenv("TEST_DIR"), "/inputs/all_uids");
    FILE *fp = fopen(uid_file, "r");
    test_assert(fp != NULL);

    uid_t uid = 0;
    char line[128];
    while (fgets(line, 128, fp)) {
        uid = atoi(line);

        /* We'll use getpwuid(3) as our reference function: */
        struct passwd *pw = getpwuid(uid);
        test_assert(pw != NULL);

        char expected_username[16];
        strncpy(expected_username, pw->pw_name, 16);
        expected_username[15] = '\0';

        char name[16] = { 'X' };
        uid_to_uname(name, uid);

        test_assert_str(name, "==", expected_username, 16);
        test_printf("'%s'", expected_username);
        test_printf("'%s'", name);
    }

    fclose(fp);
});

test_end
