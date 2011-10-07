#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include <sys/time.h>
#include <omp.h>
#include "mergesort.h"
#include "genseq.h"

START_TEST(test_correctness)
{
    uint count = 100;
    int *array = NULL;

    generate_int_sequence(&array, count);
    mergesort(array, count, sizeof(int), cmpint);

    fail_unless(check_order(array, count, sizeof(int), cmpint) == 0,
        "Error in sorting algorithm");

    free(array);
}
END_TEST

START_TEST(test_performance)
{
    uint count = 1e7;
    uint max_tries = 1;
    uint max_threads = 8;
    size_t size_in_bytes = count * sizeof(int);
    int *raw = NULL;
    int *current = malloc(size_in_bytes);

    generate_int_sequence(&raw, count);

    printf("Sorting %u elements\n", count);
    for (uint tnum = 1; tnum <= max_threads; ++tnum)
    {
        double time = 0;

        omp_set_num_threads(tnum);

        for (uint i = 0; i < max_tries; ++i)
        {
            struct timeval tv1, tv2;

            memcpy(current, raw, size_in_bytes);

            gettimeofday(&tv1, NULL);
            mergesort(current, count, sizeof(int), cmpint);
            gettimeofday(&tv2, NULL);

            time += diff_to_sec(&tv1, &tv2);

            fail_unless(check_order(current, count, sizeof(int), cmpint) == 0,
                "Error in sorting algorithm");
        }
        printf("%u threads, %.2f seconds\n", tnum, time / max_tries);
    }
    free(current);
    free(raw);
}
END_TEST

int main()
{
    Suite *suite = suite_create("MergeSort");
    TCase *tc_core = tcase_create("Core");
    SRunner *runner = srunner_create(suite);
    uint failed_count = 0;

    tcase_add_test(tc_core, test_correctness);
    tcase_add_test(tc_core, test_performance);

    tcase_set_timeout(tc_core, 0);
    suite_add_tcase(suite, tc_core);

    srunner_run_all(runner, CK_NORMAL);
    failed_count = srunner_ntests_failed(runner);
    srunner_free(runner);

    return failed_count == 0 ? EXIT_SUCCESS: EXIT_FAILURE;
}
