#ifndef MEM
#define MEM 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <cset/critical/critical.h>

const int CONTEXT_MEMORY_ADDRESS_NOT_FOUND = __COUNTER__;

/* msizeof {{{  */
#ifdef MSIZEOF
/* TODO: implement hashmap */
void **mem_storage        = NULL;
size_t *mem_storage_size  = NULL;
size_t mem_storage_count  = 0;

void mem_msizeof_cleanup() __attribute__((destructor));
void mem_msizeof_cleanup()
{
	free(mem_storage);
	free(mem_storage_size);
	mem_storage       = NULL;
	mem_storage_size  = NULL;
	mem_storage_count = 0;
}

void mem_msizeof_push(void *ptr, size_t s)
{
	size_t index = mem_storage_count++;
	size_t size = index + 1;
	mem_storage = (void **)realloc(mem_storage, size * sizeof(void *));
	critical(mem_storage, CRITICAL_OUT_OF_MEMORY);
	mem_storage_size = (size_t *)realloc(mem_storage_size, size * sizeof(size_t));
	critical(mem_storage_size, CRITICAL_OUT_OF_MEMORY);
	mem_storage[index] = ptr;
	mem_storage_size[index] = s;
}

void mem_msizeof_swap(void *n, void *o, size_t s)
{
	size_t i, len, found;
	found = 0;
	len = mem_storage_count;
	for (i = 0; i < len; i += 1) {
		if (mem_storage[i] == o) {
			mem_storage[i] = n;
			mem_storage_size[i] = s;
			found = 1;
			break;
		}
	}
	if (!found) {
		mem_msizeof_push(n, s);
	}
}

size_t msizeof(void *ptr)
{
	size_t i, len, ret;
	ret = 0;
	len = mem_storage_count;
	for (i = 0; i < len; i += 1) {
		if (mem_storage[i] == ptr) {
			ret = mem_storage_size[i];
			break;
		}
	}
	if (ret == 0 && i == len) {
		cntxt = CONTEXT_MEMORY_ADDRESS_NOT_FOUND;
	}
	return ret;
}
#endif /* MSIZEOF */
/* }}} */

void *xmem_m(size_t size)
{
	void *ptr = NULL;
       	ptr = malloc(size);
	critical(ptr, CRITICAL_OUT_OF_MEMORY);
	#ifdef MSIZEOF
	mem_msizeof_swap(ptr, ptr, size);
	#endif /* MSIZEOF */
	return ptr;
}

void *xmem_c(size_t n, size_t s)
{
	void *ptr = NULL;
	ptr = calloc(n, s);
	critical(ptr, CRITICAL_OUT_OF_MEMORY);
	#ifdef MSIZEOF
	size_t size = n * s;
	mem_msizeof_swap(ptr, ptr, size);
	#endif /* MSIZEOF */
	return ptr;
}

void *xmem_r(void *ptr, size_t s)
{
	void *old __attribute__((unused));
	old = ptr;
	ptr = realloc(ptr, s);
	critical(ptr, CRITICAL_OUT_OF_MEMORY);
	#ifdef MSIZEOF
	if (old != ptr) {
		mem_msizeof_swap(ptr, old, s);	
	} else {
		mem_msizeof_swap(ptr, ptr, s);
	}
	#endif /* MSIZEOF */
	return ptr;
}

struct mem {
	void *(*xm)(size_t);
	void *(*xc)(size_t, size_t);
	void *(*xr)(void *, size_t);

	void *(*m)(size_t);
	void *(*c)(size_t, size_t);
	void *(*r)(void *, size_t);
} mem = {
	.xm = xmem_m,
	.xc = xmem_c,
	.xr = xmem_r,

	.m = malloc,
	.c = calloc,
	.r = realloc
};

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* MEM */
