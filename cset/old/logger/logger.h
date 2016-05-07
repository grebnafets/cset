#ifndef LOGGER
#define LOGGER

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/logger/logger.linux.h>
#else
/* TODO: Segfault error. */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* LOGGER */
