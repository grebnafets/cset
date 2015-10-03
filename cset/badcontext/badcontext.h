/* To put simply, context is different form errno because we remove
 * unnecessary book keeping of constants that are inherently arbitrary.
 *
 * I personally only care about the associated definition, I never really
 * check what the constant is.
 *
 * Context doesn't really have to mean bad, so there is another variable
 * defined to signal whether the context is bad or not.
 * */

#ifndef BADCONTEXT
#define BADCONTEXT

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/* includes {{{ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cset/support/support.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

/* }}} */

#define BAD 1
#define NOTBAD 0
const int dummy_var__do_not_remove__ensures_cntxt_is_not_zero = __COUNTER__;
__thread int cntxt = 0;
__thread int bad   = 0;

void cntxtreset()
{
	bad   = 0;
	cntxt = 0;
	errno = 0;
}

void cntxterrno()
{
	int err = errno;
	if (err > 0) {
		err = -err;
		bad = 1;
	}
	cntxt = err;
}

int is(int context)
{
	return context == cntxt;
}

/* @fork {{{ */

#ifdef CAN_FORK

void badcontext_atfork()
{
	cntxt = 0;
	bad   = 0;
}

void register_badcontext_atfork() __attribute__((constructor));
void register_badcontext_atfork()
{
	pthread_atfork(NULL, NULL, &badcontext_atfork);
}

#endif /* CAN_FORK */

/* }}} */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* BADCONTEXT */
