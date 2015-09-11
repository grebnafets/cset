#include "../test/test.h"
#define TESTING
#include "critical.h"
#include "../f/f.h"

void test_critical()
{
	void *nil = NULL;
	FILE *f;
	char logstr[200];
	char cmpstr[200];
	memset(logstr, '\0', 200);
	memset(cmpstr, '\0', 200);
	size_t len;
	critical(nil, CRITICAL_OUT_OF_MEMORY);
	close_logger();
	f = fopen("logger.log", "r");
	if (f) {
		len = flen(f);
		fread(logstr, 1, len, f);
		fclose(f);
		unlink("logger.log");
	}
	sprintf(
		cmpstr,
		"%s:%s:%d:%lu:%d %s\n",
		"main.c", "test_critical", 15, pthread_self(), getpid(),
		"Out of memory"	
	);
	test(!strcmp(logstr, cmpstr));
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);
	test_critical();	
	return EXIT_SUCCESS;
}

