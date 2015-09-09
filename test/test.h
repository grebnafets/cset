#ifndef TEST
#define TEST

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../ts/ts.h"

#define TEST_SHOW_FAILURE (size_t)1
#define TEST_SHOW_SUCCESS (size_t)2
#define TEST_SHOW_RESULT (size_t)4
#define TEST_SHOW_ALL (size_t)7

size_t test_mode = 0;

__thread float test_success = 0;
__thread float test_total   = 0;

size_t test_show(size_t mode)
{
	return (test_mode & mode) == mode;
}

float test_result()
{
	return (test_success / test_total) * 100;
}

void test_set(size_t mode)
{
	test_mode = mode;
}

void test_show_result() __attribute__((destructor));
void test_show_result()
{
	if (test_show(TEST_SHOW_RESULT)) {
		if (test_total > 0) {
			fprintf(stdout, "Result: %.2f%%\n", test_result());
		} else {
			fprintf(stdout, "No test cases to run.\n");
		}
	}
}

void __test(
	int cond,
	const char *condstr __attribute__((unused)),
	const char *file __attribute__((unused)),
	const char *func __attribute__((unused)),
	int line __attribute__((unused))
)
{
	test_total++;
	if (cond) {
		test_success++;
	}
	if (cond && test_show(TEST_SHOW_SUCCESS)) {
		fprintf(
			stdout,
			"%s%s%s %-10s%d %s%lu %s%s %s%s %s%d%s\n",
			TS_BOLD, TS_GREEN,
			condstr,
			TS_PURPLE, getpid(),
			TS_BLUE, pthread_self(),
			TS_PURPLE, file,
			TS_BLUE, func,
			TS_PURPLE, line,
			TS_RESET
		);
	} else  if (!cond && test_show(TEST_SHOW_FAILURE)) {
		fprintf(
			stdout,
			"%s%s%s %-10s%d %s%lu %s%s %s%s %s%d%s\n",
			TS_BOLD, TS_RED,
			condstr,
			TS_PURPLE, getpid(),
			TS_BLUE, pthread_self(),
			TS_PURPLE, file,
			TS_BLUE, func,
			TS_PURPLE, line,
			TS_RESET
		);
	}
}

#define test(cond) __test(cond, #cond, __FILE__, __func__, __LINE__)

#endif /* TEST */
