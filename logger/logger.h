#ifndef LOGGER
#define LOGGER

#include "../badcontext/badcontext.h"
#include "../safefork/safefork.h"

pthread_mutex_t logger_lock = PTHREAD_MUTEX_INITIALIZER;
FILE *file_logger = NULL;

void open_logger() __attribute__((constructor));
void open_logger()
{
	file_logger = fopen("logger.log", "a");
}

void close_logger() __attribute__((destructor));
void close_logger()
{
	if (file_logger != NULL) {
		fclose(file_logger);
	}
	file_logger = NULL;
}

void __logger(const char *msg, const char *file, const char *func, int line)
{
	nree_wait();
	pthread_mutex_lock(&logger_lock);
	fprintf(
		file_logger,
		"%s:%s:%d:%lu:%d %s\n",
		file, func, line, pthread_self(), getpid(), msg
	);
	pthread_mutex_unlock(&logger_lock);
	nree_done();
}
#define logger(m) __logger(m, __FILE__, __func__, __LINE__)

#endif /* LOGGER */