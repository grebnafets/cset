#ifndef CSET_ERR_CROSS_PLATFORM_PRE
#define CSET_ERR_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <cset/mem/mem.h>
#include <cset/gate/gate.h>
// }}}

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_ERR_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_ERR_PRE
	#define CSET_ERR_PRE
#endif
#ifndef CSET_ERR_POST
	#define CSET_ERR_POST
#endif

char *
cset_err_set(const char *desc, const char *file, const char *func, int line)
{
CSET_ERR_PRE
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
	cset_gate_Lock(&cset_mem_lock);
	sprintf(tmp, "%d:%s:%s:%s", line, func, file, desc);
	cset_gate_Unlock(&cset_mem_lock);
	len = strlen(tmp)+1;
	cset_mem_calloc(NULL, len, sizeof(char), &err);
	cset_gate_Lock(&cset_mem_lock);
	strncpy(err, tmp, len);
	cset_gate_Unlock(&cset_mem_lock);
CSET_ERR_POST
	return err;
}
#define cset_err(desc) cset_err_set(desc, __FILE__, __func__, __LINE__)

int
cset_err_clear(char *err)
{
CSET_ERR_PRE
	int freed = 0;
	if (err != NULL) {
		cset_gate_Lock(&cset_mem_lock);
		free(err);
		cset_gate_Unlock(&cset_mem_lock);
		freed = 1;
	}
CSET_ERR_POST
	return freed;
}

#endif // CSET_ERR_CROSS_PLATFORM_PRE
