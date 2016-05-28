#ifndef CSET_THREADS
#define CSET_THREADS 1

// C++<- {{{
#ifdef __cplusplus
extern "C" {
#endif
// }}}

#include <cset/threads/threads.cp-pre.h>
/*
#ifdef __APPLE__
	#include <cset/threads/threads.apple.h>
#endif
#ifdef __WIN32__
	#include <cset/threads/threads.win32.h>
#endif
#ifdef __unix__
	#include <cset/threads/threads.unix.h>
#endif
*/
#ifdef __linux__
	#include <cset/threads/threads.linux.h>
#endif
/*
#ifdef __minix
	#include <cset/threads/threads.minix.h>
#endif
*/
#include <cset/threads/threads.cp-post.h>

// <-C++ {{{
#ifdef __cplusplus
}
#endif
// }}}

#endif // CSET_THREADS
