#ifndef CSET_TEST
#define CSET_TEST 1

// C++<- {{{
#ifdef __cplusplus
extern "C" {
#endif
// }}}

#include <cset/test/test.cp-pre.h>
#ifdef __unix__
	#include <cset/test/test.linux.h>
#endif
#include <cset/test/test.cp-post.h>

// <-C++ {{{
#ifdef __cplusplus
}
#endif
// }}}

#endif // CSET_TEST
