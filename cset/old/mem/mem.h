#ifndef MEM
#define MEM 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/mem/mem.linux.h>
#else
/* TODO: No more jokes, ran out of _memory_(babumtish). Sorry. */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* MEM */
