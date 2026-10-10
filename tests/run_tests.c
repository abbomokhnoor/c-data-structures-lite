/*
 * run_tests.c - runs every test program given on the command line and prints
 * one combined summary.   usage: run_tests <test-exe> [<test-exe> ...]
 *
 * It starts each program, echoes its output, and reads the machine-readable
 * "RESULT <tests> <checks> <failed>" line to add up the totals. Exit code is
 * 0 only if every suite passed.
 */
#define _POSIX_C_SOURCE 200809L   /* exposes popen/pclose on Linux/macOS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

int main(int argc, char **argv)
{
    int suites = 0;
    int suites_failed = 0;
    long total_tests = 0;
    long total_checks = 0;
    long total_failed_tests = 0;

    for (int i = 1; i < argc; i++) {
        char command[1024];
        if (snprintf(command, sizeof command, "\"%s\"", argv[i]) >= (int)sizeof command) {
            fprintf(stderr, "path too long: %s\n", argv[i]);
            return 2;
        }
#ifdef _WIN32
        for (char *p = command; *p != '\0'; p++) {   /* cmd.exe wants backslashes */
            if (*p == '/') {
                *p = '\\';
            }
        }
#endif
        FILE *pipe = POPEN(command, "r");
        if (pipe == NULL) {
            fprintf(stderr, "could not start %s\n", argv[i]);
            suites_failed++;
            suites++;
            continue;
        }
        int got_result = 0;
        char line[512];
        while (fgets(line, sizeof line, pipe) != NULL) {
            long t = 0;
            long c = 0;
            long f = 0;
            if (sscanf(line, "RESULT %ld %ld %ld", &t, &c, &f) == 3) {
                total_tests += t;
                total_checks += c;
                total_failed_tests += f;
                got_result = 1;
            } else {
                fputs(line, stdout);
            }
        }
        int status = PCLOSE(pipe);
        suites++;
        if (status != 0 || !got_result) {
            suites_failed++;
        }
    }

    printf("\n==============================================\n");
    printf(" Test suites : %d run, %d failed\n", suites, suites_failed);
    printf(" Tests       : %ld run, %ld failed\n", total_tests, total_failed_tests);
    printf(" Checks      : %ld evaluated\n", total_checks);
    printf(" RESULT      : %s\n", suites_failed == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    printf("==============================================\n");
    return suites_failed == 0 ? 0 : 1;
}
