#ifndef CHARMAP
#define CHARMAP 1

/* TODO: Make c++ abstraction. */

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/charmap/charmap.linux.h>
#else
/* TODO: I don't feel like it.. */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* CHARMAP */
