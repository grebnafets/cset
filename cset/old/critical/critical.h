#ifndef CRITICAL
#define CRITICAL 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/critical/critical.linux.h>
#else
/* TODO: Not right now, I have a head acne... Later. */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* CRITICAL */
