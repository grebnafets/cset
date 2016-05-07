#ifndef ASSEMBLYLINE
#define ASSEMBLYLINE 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/assemblyline/assemblyline.linux.h>
#else
// TODO: (I don't wanna!)
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* ASSEMBLYLINE */
