#ifndef CSET_ERR
#define CSET_ERR 1

// err reserves the following variable names:
//	err, error, ERROR and __line__.

// C++<- {{{
#ifdef __cplusplus
extern "C" {
#endif
// }}}

#include <cset/err/err.cp-pre.h>
/*
#ifdef __APPLE__
	#include <cset/err/err.apple.h>
#endif
#ifdef __WIN32__
	#include <cset/err/err.win32.h>
#endif
#ifdef __unix__
	#include <cset/err/err.unix.h>
#endif
#ifdef __linux__
	#include <cset/err/err.linux.h>
#endif
#ifdef __minix
	#include <cset/err/err.minix.h>
#endif
*/
#include <cset/err/err.cp-post.h>

// <-C++ {{{
#ifdef __cplusplus
}
#endif
// }}}

#endif // CSET_ERR
