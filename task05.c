#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <check.h>
#include "quicksort.h"

START_TEST(test_simple)
{
    fail_unless(1 != 0, "Fail");
}
END_TEST

int main()
{
    Suite *suite = suite_create("Sequential");
    TCase *tc_core = tcase_create("Core");
    SRunner *runner = srunner_create(suite);
    uint failed_count = 0;

    tcase_add_test(tc_core, test_simple);
    suite_add_tcase(suite, tc_core);
    srunner_run_all(runner, CK_NORMAL);
    failed_count = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed_count == 0 ? EXIT_SUCCESS: EXIT_FAILURE;
}
