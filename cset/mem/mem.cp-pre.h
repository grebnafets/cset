#ifndef CSET_MEM_CROSS_PLATFORM_PRE
#define CSET_MEM_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <cset/gate/gate.h>
// }}}

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_MEM_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_MEM_PRE
	#define CSET_MEM_PRE
#endif
#ifndef CSET_MEM_POST
	#define CSET_MEM_POST
#endif

static cset_gate_Gate cset_mem_lock;

char *
_cset_mem_err(const char *desc, const char *file, const char *func, int line)
{
CSET_MEM_PRE
	char *err = NULL;
	size_t desclen, filelen, funclen, len;
	desclen = strlen(desc);
	filelen = strlen(file);
	funclen = strlen(func);
	// I reserve 128 bytes for the representation of line a number.
	// 128 is overkill but who knows. Maybe registers size will
	// change in the future, expanding the need to reserve more
	// bytes for number repesentation.
	len = desclen + filelen + funclen + 128;
	char tmp[len];
	memset(tmp, '\0', len);
	len = strlen(tmp)+1;
	cset_gate_Lock(&cset_mem_lock);
	sprintf(tmp, "%d:%s:%s:%s", line, func, file, desc);
	cset_gate_Unlock(&cset_mem_lock);
	len = strlen(tmp)+1;
	cset_gate_Lock(&cset_mem_lock);
	err = calloc(len, sizeof(char));
	cset_gate_Unlock(&cset_mem_lock);
	if (err == NULL) { // Time to panic.
		fprintf(stderr, "ENOMEM\n");
		fflush(stderr);
		abort();
	}
	cset_gate_Lock(&cset_mem_lock);
	strncpy(err, tmp, len);
	cset_gate_Unlock(&cset_mem_lock);
CSET_MEM_POST
	return err;
}
#define cset_mem_err(desc) _cset_mem_err(desc, __FILE__, __func__, __LINE__)

char *cset_mem_malloc(char *err, size_t size, void *p)
{
CSET_MEM_PRE
	void **ptr = p;
	if (err != NULL) {
		return err;
	}
	if (*ptr != NULL) {
		err = cset_mem_err("Malloc non NULL storage is not allowed");
		return err;
	}
	cset_gate_Lock(&cset_mem_lock);
	*ptr = malloc(size);
	cset_gate_Unlock(&cset_mem_lock);
	if (*ptr == NULL) {
		err = cset_mem_err("ENOMEM");
	}
CSET_MEM_POST
	return err;
}

char *cset_mem_calloc(char *err, size_t n, size_t size, void *p)
{
CSET_MEM_PRE
	void **ptr = p;
	if (err != NULL) {
		return err;
	}
	if (*ptr != NULL) {
		err = cset_mem_err("Calloc non NULL storage is not allowed");
		return err;
	}
	cset_gate_Lock(&cset_mem_lock);
	*ptr = calloc(n, size);
	cset_gate_Unlock(&cset_mem_lock);
	if (ptr == NULL) {
		err = cset_mem_err("ENOMEM");
		return err;
	}
CSET_MEM_POST
	return err;
}

char *cset_mem_realloc(char *err, size_t size, void *p)
{
CSET_MEM_PRE
	void **ptr = p;
	if (err != NULL) {
		return err;
	}
	cset_gate_Lock(&cset_mem_lock);
	*ptr = realloc(*ptr, size);
	cset_gate_Unlock(&cset_mem_lock);
	if (ptr == NULL) {
		err = cset_mem_err("ENOMEM");
		return err;
	}
CSET_MEM_POST
	return err;
}

char *cset_mem_free(char *err, void *p)
{
CSET_MEM_PRE
	void **ptr = p;
	if (err != NULL) {
		return err;
	}
	if (*ptr == NULL) {
		err = cset_mem_err("Cannot free NULL ptr");
		return err;
	}
	cset_gate_Lock(&cset_mem_lock);
	free(*ptr);
	cset_gate_Unlock(&cset_mem_lock);
	*ptr = NULL;
	return err;
CSET_MEM_POST
}

#endif // CSET_MEM_CROSS_PLATFORM_PRE
