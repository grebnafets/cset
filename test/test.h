#ifndef TEST
#define TEST

/* Reserve words for testing. {{{ */
/* EQUALS */
#define EQU ==
/* NOT EQUALS */
#define NOT !=
/* LESS THAN EQUALS */
#define LTE <=
/* GREATER THAN EQUALS */
#define GTE >=
/* LESS THAN */
#define LT <
/* GREATER THAN */
#define GT >
/* }}} */

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/* includes {{{ */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../ts/ts.h"
/* }}} */

/* modes {{{ */
#define TEST_SHOW_FAILURE (size_t)1
#define TEST_SHOW_SUCCESS (size_t)2
#define TEST_SHOW_RESULT (size_t)4
#define TEST_SHOW_ALL (size_t)7
size_t test_mode = 0;
/* }}} */

/* vars {{{ */
char test_name_main[100];
size_t test_name_displayed  = 0;
__thread float test_success = 0;
__thread float test_total   = 0;
/* }}} */


/* functions {{{ */

void test_init() __attribute__((constructor));
void test_init()
{
	memset(test_name_main, '\0', 100);
}

/* Gives you mode from main arg. */
size_t test_get_mode_from_arg(int argc, char **argv)
{
	char *p;
	size_t mode = 0;
	if (argc == 2) {mode = strtoul(argv[1], &p, 10);}
	return mode;
}

/* Will tell you if the provided mode is on or not. */
size_t test_mode_is(size_t mode)
{
	return (test_mode & mode) == mode;
}

/* Get results. */
float test_result()
{
	return (test_success / test_total) * 100;
}

/* Set stuff */
void test_set_mode(size_t mode)
{
	test_mode = mode;
}
void test_set_name(const char *name)
{
	strcpy(test_name_main, name);
}
void __test_set(size_t argcount, ...)
{
	size_t mode;
	const char *name;
	va_list argp;
	va_start(argp, argcount);
	if (argcount == 1) {
		mode = va_arg(argp, size_t);
		test_set_mode(mode);
	} else if (argcount == 2) {
		mode = va_arg(argp, size_t);
		name = va_arg(argp, const char *);
		test_set_mode(mode);
		test_set_name(name);
	}
	va_end(argp);
}
#define TEST_SET_ARGNUM_HACKLEN(_2, _1, N, ...) N
#define TEST_SET_ARGNUM(args...) TEST_SET_ARGNUM_HACKLEN(args, 2, 1, 0)
#define test_set(args...) __test_set(TEST_SET_ARGNUM(args), args)

/* Run the tests @ end of main. */
#ifndef TEST_DONT_RUN_AT_END_OF_MAIN
void test_show_result() __attribute__((destructor));
#endif /* TEST_DONT_RUN_AT_END_OF_MAIN */

/* Display results if result flag is on. */
void test_show_result()
{
	if (test_mode_is(TEST_SHOW_RESULT)) {
		if (test_total > 0) {
			if (test_name_main[0] != '\0') {
				fprintf(
					stdout,
					"%s: %.2f%%\n",
					test_name_main, test_result()
				);
			} else {
				fprintf(
					stdout,
					"%s: %.2f%%\n",
					"Anon", test_result()
				);
			}
		} else {
#ifndef TEST_SILENT_IF_NO_TESTS
			fprintf(stdout, "No test cases to run.\n");
#endif /* TEST_SILENT_IF_NO_TESTS */
		}
	}
}

/* test implementation. */
void __test(
	int cond,
	const char *condstr __attribute__((unused)),
	const char *file __attribute__((unused)),
	const char *func __attribute__((unused)),
	int line __attribute__((unused))
)
{

	/* define len, assert, info {{{ */
	int len = (strlen(condstr) + 10) * sizeof(char);
	if (len < 40) {
		len = 40;
	} else if (len < 60) {
		len = 60;
	}
	char assert[len];
	char info[512];
	/* }}} */

	/* Increment total number of tests and success if success {{{ */
	test_total++;
	if (cond) {
		test_success++;
	}
	/* }}} */

	/* Prepare info {{{ */
	sprintf(
		info,		
		"%s%s%04d %s%s %s%s %s%lu %s%d%s\n",
		TS_BOLD,
		TS_PURPLE, line,
		TS_BLUE, func,
		TS_PURPLE, file,
		TS_BLUE, pthread_self(),
		TS_PURPLE, getpid(),
		TS_RESET
	);
	/* }}} */

	/* Display results {{{ */
	if (cond && test_mode_is(TEST_SHOW_SUCCESS)) {
		sprintf(
			assert,
			" %s%s%s%s",
			TS_BOLD, TS_GREEN,
			condstr,
			TS_RESET	
		);
		fprintf(stdout,"%-*s %s", len + 1, assert, info);
	} else  if (!cond && test_mode_is(TEST_SHOW_FAILURE)) {
		sprintf(
			assert,
			" %s%s%s%s",
			TS_BOLD, TS_RED,
			condstr,
			TS_RESET	
		);
		fprintf(stdout,"%-*s %s", len + 1, assert, info);
	}
	/* }}} */

}
/* }}} */

/* Hook it, and eat ice cream. */
#define test(cond) __test(cond, #cond, __FILE__, __func__, __LINE__)

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* TEST */
