#include <cset/test/test.h>
#include <cset/logger/logger.h>
#include <cset/f/f.h>

void test_logger()
{
	FILE *f;
	char logstr[200];
	char cmpstr[200];
	memset(logstr, '\0', 200);
	memset(cmpstr, '\0', 200);
	size_t len;
	logger("dis is log");
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
		__FILE__, "test_logger", 13, pthread_self(), getpid(),
		"dis is log"	
	);
	test(!strcmp(logstr, cmpstr));
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "logger");
	test_logger();
}
