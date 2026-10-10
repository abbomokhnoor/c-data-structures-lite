/*
 * test_framework.h - a tiny self-written unit-test framework (header only).
 *
 * HOW IT WORKS
 *   - A test is a function:  static void test_something(void) { ... }
 *   - Inside it, use CHECK(...) macros. A failed check prints the file, line
 *     and expression, marks the current test as failed, and keeps going.
 *   - main() lists the tests with RUN_TEST(fn) and ends with
 *     `return TEST_REPORT("module name");`
 *   - The exit code is 0 when everything passed, 1 otherwise.
 *   - The last line printed is machine-readable ("RESULT <tests> <checks>
 *     <failed-tests>") so run_tests.c can add up all suites.
 *
 * MINIMAL EXAMPLE
 *   #include "test_framework.h"
 *   static void test_math(void) { CHECK_EQ_INT(2 + 2, 4); }
 *   int main(void) { RUN_TEST(test_math); return TEST_REPORT("math"); }
 */
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

static int tf_tests_run = 0;       /* number of test functions executed   */
static int tf_tests_failed = 0;    /* number of test functions with a failure */
static int tf_checks_run = 0;      /* total CHECK macros evaluated        */
static int tf_current_failed = 0;  /* did the running test fail so far?   */

/* Core check: `cond` must be true. */
#define CHECK(cond)                                                          \
    do {                                                                     \
        tf_checks_run++;                                                     \
        if (!(cond)) {                                                       \
            printf("    FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);       \
            tf_current_failed = 1;                                           \
        }                                                                    \
    } while (0)

/* Compare two integers; prints both values when they differ. */
#define CHECK_EQ_INT(actual, expected)                                       \
    do {                                                                     \
        long long tf_a = (long long)(actual);                                \
        long long tf_e = (long long)(expected);                              \
        tf_checks_run++;                                                     \
        if (tf_a != tf_e) {                                                  \
            printf("    FAIL %s:%d: %s == %s (got %lld, expected %lld)\n",   \
                   __FILE__, __LINE__, #actual, #expected, tf_a, tf_e);      \
            tf_current_failed = 1;                                           \
        }                                                                    \
    } while (0)

/* Compare two C strings. */
#define CHECK_STR_EQ(actual, expected)                                       \
    do {                                                                     \
        const char *tf_sa = (actual);                                        \
        const char *tf_se = (expected);                                      \
        tf_checks_run++;                                                     \
        if (tf_sa == NULL || tf_se == NULL || strcmp(tf_sa, tf_se) != 0) {   \
            printf("    FAIL %s:%d: %s == %s (got \"%s\", expected \"%s\")\n",\
                   __FILE__, __LINE__, #actual, #expected,                   \
                   tf_sa ? tf_sa : "(null)", tf_se ? tf_se : "(null)");      \
            tf_current_failed = 1;                                           \
        }                                                                    \
    } while (0)

/* Compare a dsa_status return value with the status we expect. */
#define CHECK_STATUS(actual, expected) CHECK_EQ_INT((actual), (expected))

/* Compare two int arrays element by element. */
#define CHECK_ARRAY_EQ(actual, expected, n)                                  \
    do {                                                                     \
        size_t tf_n = (size_t)(n);                                           \
        int tf_same = 1;                                                     \
        tf_checks_run++;                                                     \
        for (size_t tf_i = 0; tf_i < tf_n; tf_i++) {                         \
            if ((actual)[tf_i] != (expected)[tf_i]) {                        \
                printf("    FAIL %s:%d: %s[%zu] = %d, expected %d\n",        \
                       __FILE__, __LINE__, #actual, tf_i,                    \
                       (actual)[tf_i], (expected)[tf_i]);                    \
                tf_same = 0;                                                 \
                break;                                                       \
            }                                                                \
        }                                                                    \
        if (!tf_same) {                                                      \
            tf_current_failed = 1;                                           \
        }                                                                    \
    } while (0)

/* Run one test function and print a PASS/FAIL line for it. */
#define RUN_TEST(fn)                                                         \
    do {                                                                     \
        tf_current_failed = 0;                                               \
        tf_tests_run++;                                                      \
        fn();                                                                \
        if (tf_current_failed) {                                             \
            tf_tests_failed++;                                               \
            printf("  [FAIL] %s\n", #fn);                                    \
        } else {                                                             \
            printf("  [ OK ] %s\n", #fn);                                    \
        }                                                                    \
    } while (0)

/* Print the summary for one test program; evaluates to the exit code. */
#define TEST_REPORT(suite_name)                                              \
    (printf("%s: %d/%d tests passed (%d checks)\n", (suite_name),            \
            tf_tests_run - tf_tests_failed, tf_tests_run, tf_checks_run),    \
     printf("RESULT %d %d %d\n", tf_tests_run, tf_checks_run,                \
            tf_tests_failed),                                                \
     tf_tests_failed == 0 ? 0 : 1)

#endif /* TEST_FRAMEWORK_H */
