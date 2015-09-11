#ifndef CRITICAL
#define CRITICAL

#include "../logger/logger.h"

#define CRITICAL_OUT_OF_MEMORY 1

void __critical(
	void *val, int type_of_check,
	const char *file, const char *func, int line	
)
{
	switch(type_of_check) {
	case CRITICAL_OUT_OF_MEMORY:
		if (val == NULL) {
			#ifndef TESTING
			fprintf(
				stderr,
				"Out of memory@%s:%s:%d\n",
				file, func, line		
			);
			#endif /* TESTING */
			__logger("Out of memory", file, func, line);
			goto FATAL;
		}
		break;
	}
	goto NO_ERROR;
FATAL:
	close_logger();
	#ifndef TESTING
	abort();
	#endif /* TESTING */
NO_ERROR:
	return;
}
#define critical(v, t) __critical(v, t, __FILE__, __func__, __LINE__)

#endif /* CRITICAL */
