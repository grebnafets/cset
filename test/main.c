#include "test.h"

void some_testcase()
{
	int foo, bar;
	foo = 1;
	bar = 2;

	test(foo NOT bar);
	test(foo != bar);

	test(bar GT foo);
	test(bar > foo);

	test(foo LT bar);
	test(foo < bar);


	test(foo EQU bar);
	test(foo == bar);

	test(bar LTE foo);
	test(bar <= foo);

	test(foo GTE bar);
	test(foo >= bar);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "test");
	some_testcase();
	return EXIT_SUCCESS;
}
