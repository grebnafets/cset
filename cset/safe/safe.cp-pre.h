#ifndef CSET_SAFE_CROSS_PLATFORM_PRE
#define CSET_SAFE_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <cset/gate/gate.h>
// }}}

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_SAFE_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_SAFE_PRE
	#define CSET_SAFE_PRE
#endif
#ifndef CSET_SAFE_POST
	#define CSET_SAFE_POST
#endif

void *cset_safe_malloc(size_t size)
{
CSET_SAFE_PRE
	void *ptr = NULL;
	cset_gate_Lock(&cset_gate_Mem);
	ptr = malloc(size);
	cset_gate_Unlock(&cset_gate_Mem);
CSET_SAFE_POST
	return ptr;
}

void *cset_safe_realloc(void *ptr, size_t size)
{
CSET_SAFE_PRE
	cset_gate_Lock(&cset_gate_Mem);
	ptr = realloc(ptr, size);
	cset_gate_Unlock(&cset_gate_Mem);
CSET_SAFE_POST
	return ptr;
}

void *cset_safe_calloc(size_t n, size_t size)
{
CSET_SAFE_PRE
	void *ptr = NULL;
	cset_gate_Lock(&cset_gate_Mem);
	ptr = calloc(n, size);
	cset_gate_Unlock(&cset_gate_Mem);
CSET_SAFE_POST
	return ptr;
}

void cset_safe_free(void *ptr)
{
CSET_SAFE_PRE
	cset_gate_Lock(&cset_gate_Mem);
	free(ptr);
	cset_gate_Unlock(&cset_gate_Mem);
CSET_SAFE_POST
}

#define cset_safe_sprintf(ret, args...) do {					\
	cset_gate_Lock(&cset_gate_Mem);						\
	ret = sprintf(args);							\
	cset_gate_Unlock(&cset_gate_Mem);					\
} while(0)

#endif // CSET_SAFE_CROSS_PLATFORM_PRE
