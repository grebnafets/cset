#include <cset/test/test.h>
#include <cset/f/f.h>

void test_flen()
{
	FILE *f = NULL;
	size_t len = 0;
	f = fopen("f.txt", "r");
	len = flen(f);
	test(len == 13);
	fclose(f);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);
	test_flen();
	return EXIT_SUCCESS;
}
