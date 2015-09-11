#include "../test/test.h"
#define MSIZEOF
#include "mem.h"

void test_mem()
{
	void *t1 = mem.xm(10);
	test(msizeof(t1) == 10);
	t1 = mem.xr(t1, 20);
	printf("%lu\n", msizeof(t1));
	test(msizeof(t1) == 20);
	free(t1);
	t1 = mem.xc(1, 2);
	test(msizeof(t1) == 2);
	free(t1);
}

void test_use_valgrind()
{
	void *t1 = mem.m(10);
	t1 = mem.r(t1, 12);
	free(t1);
	t1 = mem.c(1, 2);
	free(t1);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);
	test_mem();
	test_use_valgrind();
}
