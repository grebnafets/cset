#ifndef CSET_ERR_CROSS_PLATFORM_PRE
#define CSET_ERR_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <string.h>
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
cset_err_set(const char *desc, const char *file, const char *func, int __line__)
{
CSET_ERR_PRE
	char *err = NULL;
	size_t desclen, filelen, funclen, len;
	desclen = strlen(desc);
	filelen = strlen(file);
	funclen = strlen(func);
	// I reserve 128 bytes for the representation of __line__ a number.
	// 128 is overkill but who knows. Maybe registers size will
	// change in the future, expanding the need to reserve more
	// bytes for number repesentation.
	len = desclen + filelen + funclen + 128;
	char tmp[len];
	memset(tmp, '\0', len);
	cset_gate_Lock(&cset_gate_Mem);
	sprintf(tmp, "%d:%s:%s:%s", __line__, func, file, desc);
	cset_gate_Unlock(&cset_gate_Mem);
	len = strlen(tmp)+1;
	cset_gate_Lock(&cset_gate_Mem);
	err = calloc(len, sizeof(char));
	strncpy(err, tmp, len);
	cset_gate_Unlock(&cset_gate_Mem);
CSET_ERR_POST
	return err;
}
#define cset_err(desc) cset_err_set(desc, __FILE__, __func__, __line__)

int
cset_err_clear(char *err)
{
CSET_ERR_PRE
	int freed = 0;
	if (err != NULL) {
		cset_gate_Lock(&cset_gate_Mem);
		free(err);
		cset_gate_Unlock(&cset_gate_Mem);
		freed = 1;
	}
CSET_ERR_POST
	return freed;
}

#define INITERR char *err = NULL
#define ERROR char *err, int __line__
#define error err, __LINE__

#endif // CSET_ERR_CROSS_PLATFORM_PRE
