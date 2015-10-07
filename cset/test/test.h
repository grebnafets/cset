#ifndef TEST
#define TEST

/* Reserve words for testing. {{{ */
/* EQUALS */
#define EQU ==
#define IS ==
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
/* DEPENDENCY FOUND */
#define DEPENDENCY 1
#define DEP 1
#define FOUND ==
/* }}} */

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/* includes {{{ */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <cset/ts/ts.h>
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

void test_memfail(void *ptr)
{
	if (ptr == NULL) {
		fprintf(stderr, "@test.h: Out of mem\n");
		exit(EXIT_FAILURE);
	}
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
					"%s:%d:%lu %.2f%%\n",
					test_name_main,
					getpid(),
					pthread_self(),
				       	test_result()
				);
			} else {
				fprintf(
					stdout,
					"%s:%d:%lu %.2f%%\n",
					"Anon",
					getpid(),
					pthread_self(),
					test_result()
				);
			}
		} else {
			#ifndef TEST_SILENT_IF_NO_TESTS
			fprintf(stdout, "No test cases to run.\n");
			#endif /* TEST_SILENT_IF_NO_TESTS */
		}
	}
}

/* hook threads {{{ */
typedef void *(*TEST_PTHREAD_HOOK)(void *);
TEST_PTHREAD_HOOK *test_hooks = NULL;
void **test_hooks_arg = NULL;
size_t *test_hooks_id = NULL;
size_t test_hooks_len = 0;

void *test_wrapped_thread(void *arg)
{
	size_t i = (size_t)arg;
	void *res = test_hooks[i](test_hooks_arg[i]);
	test_show_result();
	return res;
}

void test_pthread_cleanup() __attribute__((destructor));
void test_pthread_cleanup()
{
	free(test_hooks);
	free(test_hooks_arg);
	free(test_hooks_id);
	test_hooks = NULL;
	test_hooks_arg = NULL;
	test_hooks_id = NULL;
	test_hooks_len = 0;
}

int test_pthread_create(
	pthread_t *thread,
	const pthread_attr_t *attr,
	void *(*sr)(void *), void *arg)
{
	size_t index = test_hooks_len++;
	test_hooks = (TEST_PTHREAD_HOOK *)realloc(
		test_hooks, test_hooks_len * sizeof(TEST_PTHREAD_HOOK)
	);
	test_memfail(test_hooks);
	test_hooks_id = (size_t *)realloc(
		test_hooks_id, test_hooks_len * sizeof(size_t *)
	);
	test_memfail(test_hooks_id);
	test_hooks_arg = (void **)realloc(
		test_hooks_arg, test_hooks_len * sizeof(void *)
	);
	test_memfail(test_hooks_arg);
	test_hooks[index] = sr;
	test_hooks_arg[index] = arg;
	test_hooks_id[index] = index;
	return pthread_create(
		thread, attr, &test_wrapped_thread, (void *)test_hooks_id[index]
	);
}
#undef pthread_create
#define pthread_create(t, at, r, a) test_pthread_create(t, at, r, a)
/* }}} */

void test_init() __attribute__((constructor));
void test_init()
{
	memset(test_name_main, '\0', 100);
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
#define testdelicate(cond, file, f, l) __test(cond, #cond, file, f, l)

/* 
 * When testing becomes hard, this offers a dirty but quick and effective way
 * to make it easy again.
 * */
/* test diff {{{ */
char **test_fexpected = NULL;
char **test_fresult   = NULL;
size_t test_diff_count = 0;

void test_add_diff(const char *expected, const char *result)
{
	size_t len, index;
        len = 0;
	index = test_diff_count++;
	test_fexpected = (char **)realloc(
		test_fexpected, test_diff_count * sizeof(char *)
	);
	len = strlen(expected);
	test_fexpected[index] = (char *)calloc(len + 1, sizeof(char));
	strcpy(test_fexpected[index], expected);

	test_fresult = (char **)realloc(
		test_fresult, test_diff_count * sizeof(char *)
	);
	len = strlen(result);
	test_fresult[index] = (char *)calloc(len + 1, sizeof(char));
	strcpy(test_fresult[index], result);
}

void __test_diff(
	const char *file __attribute__((unused)),
	const char *func __attribute__((unused)),
	int line __attribute__((unused))	
)
{
	size_t i, len, l;
	len = test_diff_count;
	FILE *f1, *f2;
	char lexpected[1024];
	char lresult[1024];
	char *expected;
	char *result;
	l = 1;
	for (i = 0; i < len; i += 1) {
		f1 = fopen(test_fexpected[i], "r");
		if (ferror(f1)) {perror(strerror(errno));abort();}
		f2 = fopen(test_fresult[i], "r");
		if (ferror(f2)) {perror(strerror(errno));abort();}
		memset(lexpected, '\n', sizeof(lexpected));
		memset(lresult, '\n', sizeof(lresult));
		while (1) {
			expected = fgets(lexpected, sizeof(lexpected), f1);
			result = fgets(lresult, sizeof(lresult), f2);
			if (expected == NULL && result == NULL) {break;}
			if (expected == NULL) {
				testdelicate(
					expected NOT NULL,
					file, func, line
				);
				break;
			} else if (result == NULL) {
				testdelicate(
					result NOT NULL,
					file, func, line
				);
				break;
			}
			if(0
				|| test_mode_is(TEST_SHOW_SUCCESS)
				|| test_mode_is(TEST_SHOW_FAILURE)
			) {
				fprintf(stdout, " %lu", l);
			}
			testdelicate(
				!strcmp(lexpected, lresult),
				file, func, line
			);
			memset(lexpected, '\n', sizeof(lexpected));
			memset(lresult, '\n', sizeof(lresult));
			l++;
		}
		fclose(f1);
		fclose(f2);
	}

}
#define test_diff() __test_diff(__FILE__, __func__, __LINE__)

/* }}} test diff */

void test_destructor() __attribute__((destructor));
void test_destructor()
{
	size_t i, len;
	len = test_diff_count;
	for (i = 0; i < len; i += 1) {
		free(test_fexpected[i]);
	}
	free(test_fexpected);
	for (i = 0; i < len; i += 1) {
		free(test_fresult[i]);
	}
	free(test_fresult);

}

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* TEST */
