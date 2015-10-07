#ifndef SUPPORT_LINUX
#define SUPPORT_LINUX 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Macro def {{{ */

#define CAN_FORK 0
#define CAN_SIGACTION 0

/* }}} */

/* Macro init {{{1 */

/* fork {{{2 */
#ifdef __CYGWIN__
	#undef CAN_FORK
	#define CAN_FORK 1
	/* 
	 * TODO: check support under cygwin
	 * #undef CAN_SIGACTION
	 * #define CAN_SIGACTION 1
	 *
	 * */
#endif /* __CYGWIN__ */

#ifdef __linux__
	#undef CAN_FORK
	#define CAN_FORK 1
	#undef CAN_SIGACTION
	#define CAN_SIGACTION 1
#endif /* __linux__ */

#ifdef __unix__
	#undef CAN_FORK
	#define CAN_FORK 1
	#undef CAN_SIGACTION
	#define CAN_SIGACTION 1
#endif /* __unix__ */

/* }}}2 */

/* }}}1 */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* SUPPORT_LINUX */
