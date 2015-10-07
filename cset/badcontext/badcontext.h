/* To put simply, context is different form errno because we remove
 * unnecessary book keeping of constants that are inherently arbitrary.
 *
 * I personally only care about the associated definition, I never really
 * check what the constant is.
 *
 * Context doesn't really have to mean bad, so there is another variable
 * defined to signal whether the context is bad or not.
 * */

#ifndef BADCONTEXT
#define BADCONTEXT 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifdef __unix__
#include <cset/badcontext/badcontext.linux.h>
#else
/*TODO: not now... */
#endif

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* BADCONTEXT */
