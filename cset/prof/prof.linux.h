#ifndef PROF_LINUX
#define PROF_LINUX 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <stdio.h>
#include <time.h>
#include <cset/ts/ts.h>

void prof_sec(clock_t start, const char *name)
{
	clock_t stop = clock();
	double res = (double)(stop - start) / CLOCKS_PER_SEC;
	fprintf(
		stdout,
		"%s%s%-15s%s ... %s%s%f%s\n",
		TS_YELLOW, TS_BOLD,
		name,
		TS_RESET,
		TS_GREEN, TS_BOLD,
		res,
		TS_RESET
	);
}

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* PROF_LINUX */
