#ifndef SUPPORT
#define SUPPORT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Macro def {{{ */

#define CAN_FORK 0

/* }}} */

/* Macro init {{{1 */

/* fork {{{2 */
#ifdef __CYGWIN__
	#undef CAN_FORK
	#define CAN_FORK 1
#endif /* __CYGWIN__ */

#ifdef __linux__
	#undef CAN_FORK
	#define CAN_FORK 1
#endif /* __linux__ */

#ifdef __unix__
	#undef CAN_FORK
	#define CAN_FORK 1
#endif /* __unix__ */
/* }}}2 */

/* }}}1 */

#endif /* SUPPORT */
