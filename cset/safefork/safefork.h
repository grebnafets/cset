/* nree = Non-RE-Entry (method)
 *
 * This header file is far from complete..
 * Only malloc, calloc, realloc and free have been made "safe" so far.
 * It is _NOT_ enough to just include the header file, you have to tell
 * it to overwrite the functions first with the "SAFEFORK_HOOK" macro.
 * You currently can overwrite:
 * [fork, malloc, realloc, calloc and free] by defining "SAFEFORK_HOOK" macro.
 * That will hook target -> with:
 * fork -> fork_safe
 * [malloc, calloc, realloc, free] -> nree_*[malloc, calloc, realloc, free]
 *
 * "safefork.h" provides you with lockfork, unlockfork, locknree, unlocknree,
 * nree_wait and nree_done functions. The _wait and _done functions will
 * automatically call lock and unlock. forklock will only work for the
 * function fork_safe, so you might want to hook it if you didn't or don't
 * want to define the "SAFEFORK_HOOK" macro. See nree_malloc below to see
 * these functions in use.
 * */

#ifndef SAFEFORK
#define SAFEFORK 1
#define FORK
#define NREE

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <cset/support/support.h>

#ifdef CAN_FORK

/* include {{{ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <cset/badcontext/badcontext.h>
/* }}} */

/* Profile layer {{{ */

#ifndef PS
	#define PS
#endif /* PS */
#ifndef PE
	#define PE
#endif /* PE */

/* }}} */

/* context {{{1 */

/* bad {{{2 */
#define SAFEFORK_FORK_CLEANUP 2
const int CONTEXT_BAD_FAILED_TO_CREATE_FORK = __COUNTER__;
/* }}}2 */

/* }}}1 */

/* spin locks {{{ */
#ifndef FORK_SPIN
	#define FORK_SPIN
#endif /* FORK_SPIN */

#ifndef NREE_SPIN
	#define NREE_SPIN
#endif /* NREE_SPIN */

#ifndef SAFEFORK_SPIN
	#define SAFEFORK_SPIN
#endif /* SAFEFORK_SPIN */

/* }}} */

long lock_fork = 0;
long lock_nree = 0;

void lockfork()
{
	lock_fork++;
}
void unlockfork()
{
	lock_fork--;
}
void locknree()
{
	lock_nree++;
}
void unlocknree()
{
	lock_nree--;
}

/* fork {{{ */
void fork_wait()
{
	PS;
	while (lock_fork > 0) {FORK_SPIN;}
	locknree();
	PE;
}

void fork_done()
{
	PS;
	unlocknree();
	PE;
}

pid_t fork_safe()
{
	PS;
	fork_wait();
	SAFEFORK_SPIN;
	pid_t id = fork();
	if (id == -1) {
		cntxt = CONTEXT_BAD_FAILED_TO_CREATE_FORK;
		bad   = BAD;
	}
	PE;
	return id;
}

void fork_atfork() __attribute__((constructor));
void fork_atfork()
{
	PS;
	pthread_atfork(NULL, &fork_done, &fork_done);
	PE;	
}

void fork_cleanup()
{
	PS;
	int status;
	pid_t w;
	for (;;) {
		w = wait(&status);
		if (w == -1 && errno == ECHILD) {break;}
		if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
			cntxt = status;
			bad   = BAD + SAFEFORK_FORK_CLEANUP;
			break;
		}
	}
	PE;
}

/* }}} */

/* nree {{{ */

void nree_wait()
{
	PS;
	while(lock_nree > 0) {NREE_SPIN;}
	lockfork();
	PE;
}

void nree_done()
{
	PS;
	unlockfork();
	PE;
}

void *nree_malloc(size_t size)
{
	PS;
	void *ptr = NULL;
	nree_wait();
	ptr = malloc(size);
	SAFEFORK_SPIN;
	nree_done();
	PE;
	return ptr;
}

void *nree_calloc(size_t unit_count, size_t unit_size)
{
	PS;
	void *ptr = NULL;
	nree_wait();
	ptr = calloc(unit_count, unit_size);
	nree_done();
	PE;
	return ptr;	
}

void *nree_realloc(void *ptr, size_t size)
{
	PS;
	nree_wait();
	ptr = realloc(ptr, size);
	SAFEFORK_SPIN;
	nree_done();
	PE;
	return ptr;
}

void nree_free(void *ptr)
{
	PS;
	nree_wait();
	free(ptr);
	SAFEFORK_SPIN;
	nree_done();
	PE;
}

int nree_pthread_mutex_lock(pthread_mutex_t *mutex)
{
	int ret;
	PS;
	nree_wait();
	ret = pthread_mutex_lock(mutex);	
	nree_done();
	PE;
	return ret;
}

int nree_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	int ret;
	PS;
	nree_wait();
	ret = pthread_mutex_unlock(mutex);	
	nree_done();
	PE;
	return ret;
}

/* }}} */

/* hooks {{{ */
#ifdef SAFEFORK_HOOK

	#define fork() fork_safe()
	#define malloc(s) nree_malloc(s)
	#define calloc(n, s) nree_calloc(n, s)
	#define realloc(p, s) nree_realloc(p, s)
	#define free(p) nree_free(p)
	#define pthread_mutex_lock(m) nree_pthread_mutex_lock(m)
	#define pthread_mutex_unlock(m) nree_pthread_mutex_unlock(m)

#endif /* SAFEFORK_HOOK */
/* }}} */

/* Undefine profile layer {{{ */

#undef PS
#undef PE

/* }}} */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* CAN_FORK */
#endif /* SAFEFORK */
