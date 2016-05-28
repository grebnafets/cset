#ifndef CSET_THREADS_CROSS_PLATFORM_PRE
#define CSET_THREADS_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <cset/gate/gate.h>
#include <cset/mem/mem.h>
#include <cset/err/err.h>
// }}}

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_THREADS_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_THREADS_PRE
	#define CSET_THREADS_PRE
#endif
#ifndef CSET_THREADS_POST
	#define CSET_THREADS_POST
#endif


#endif // CSET_THREADS_CROSS_PLATFORM_PRE
