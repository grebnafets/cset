#ifndef THREADWRAPP
#define THREADWRAPP 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/threadwrapp/threadwrapp.linux.h>
#else
/* TODO: ... */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* THREADWRAPP */
