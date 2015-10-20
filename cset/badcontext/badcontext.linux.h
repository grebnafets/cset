#ifndef BADCONTEXT_LINUX
#define BADCONTEXT_LINUX 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/* includes {{{ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
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
#ifdef DEBUG
__thread char *badcontextstr;
#endif

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

void (*segfault_handle)(void) = NULL;

void segfault_sigaction(
	int signal __attribute__((unused)),
	siginfo_t *si __attribute__((unused)),
	void *arg __attribute__((unused))
)
{
	fprintf(stderr, "Segfault detected.\n");
	if (segfault_handle != NULL) {
		segfault_handle();
	}
	abort();
}


/* @fork {{{ */

#if CAN_FORK
void badcontext_atfork()
{
	cntxt = 0;
	bad   = 0;
}
#endif /* CAN_FORK */

void badcontext_init() __attribute__((constructor));
void badcontext_init()
{
#if CAN_FORK
	pthread_atfork(NULL, NULL, &badcontext_atfork);
#endif

#if CAN_SIGACTION
#ifndef NO_SIGACTION
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = &segfault_sigaction;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);
#endif
#endif
}

/* }}} */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* BADCONTEXT_LINUX */
