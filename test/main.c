#include "test.h"

void some_testcase()
{
	int foo, bar;
	foo = 1;
	bar = 2;
	test(foo != bar);
	test(bar >= foo);
	test(foo <= bar);

	test(foo == bar);
	test(bar <= foo);
	test(foo >= bar);	
}

int main(int argc, char **argv)
{
	char *p;
	size_t mode = 0;
	if (argc == 2) {mode = strtoul(argv[1], &p, 10);}
	test_set(mode);
	some_testcase();
	return EXIT_SUCCESS;
}
