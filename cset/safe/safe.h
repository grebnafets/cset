#ifndef CSET_SAFE
#define CSET_SAFE 1

// safe reserves the following variable names:
//	safe, safeor, SAFEOR and __line__.

// C++<- {{{
#ifdef __cplusplus
extern "C" {
#endif
// }}}

#include <cset/safe/safe.cp-pre.h>
/*
#ifdef __APPLE__
	#include <cset/safe/safe.apple.h>
#endif
#ifdef __WIN32__
	#include <cset/safe/safe.win32.h>
#endif
#ifdef __unix__
	#include <cset/safe/safe.unix.h>
#endif
*/
#ifdef __linux__
	#include <cset/safe/safe.linux.h>
#endif
/*
#ifdef __minix
	#include <cset/safe/safe.minix.h>
#endif
*/
#include <cset/safe/safe.cp-post.h>

// <-C++ {{{
#ifdef __cplusplus
}
#endif
// }}}

#endif // CSET_SAFE
