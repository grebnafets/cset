#ifndef HASHMAP
#define HASHMAP 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/hashmap/hashmap.linux.h>
#else
/* TODO: Blue screen of deeeeeeaaaatthhhhhhhhh. */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* HASHMAP */
