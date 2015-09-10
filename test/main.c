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
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);
	some_testcase();
	return EXIT_SUCCESS;
}
