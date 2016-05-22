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

static cset_gate_Gate cset_err_gate = 0;

char *
cset_err_set(const char *desc, const char *func, int line)
{CSET_ERR_PRE
	cset_gate_Lock(&cset_err_gate);
	char *err = NULL;
	size_t len = strlen(desc)+128;
	if (len == 0 || len > 9000 || line <= 0 || strlen(func) > 9000) {
		// Will only run if you are not being nice.
		cset_mem_calloc(NULL, strlen("ErrLvOver9000")+128, sizeof(char), &err);
		if (err == NULL) {
			fprintf(stderr, "ENOMEM\n");
			fflush(stderr);
			abort();
		}
		sprintf(err, "%d:%s", line, "ErrLvOver9000");
		return err;
	}
	char tmp[len];
	memset(tmp, '\0', len);
	sprintf(tmp, "%d:%s:%s", line, func, desc);
	len = strlen(tmp)+1;
	cset_mem_calloc(NULL, len, sizeof(char), &err);
	strncpy(err, tmp, len);
	cset_gate_Unlock(&cset_err_gate);
CSET_ERR_POST
	return err;
}
#define cset_err(desc) cset_err_set(desc, __func__, __LINE__)

int
cset_err_clear(char *err)
{CSET_ERR_PRE
	cset_gate_Lock(&cset_err_gate);
	int freed = 0;
	if (err != NULL) {
		free(err);
		freed = 1;
	}
	cset_gate_Unlock(&cset_err_gate);
CSET_ERR_POST
	return freed;
}

#endif // CSET_ERR_CROSS_PLATFORM_PRE
