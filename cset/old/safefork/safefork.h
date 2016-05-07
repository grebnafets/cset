#ifndef SAFEFORK
#define SAFEFORK 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/safefork/safefork.linux.h>
#else
/* TODO: Who, me? */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* SAFEFORK */
