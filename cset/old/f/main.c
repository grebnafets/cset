#include <cset/test/test.h>
#include <cset/f/f.h>

/*
 * TODO: Test bigger file sizes.
 * */

/* flen {{{ */
void test_flen_safe()
{
	FILE *f = NULL;
	size_t len = 0;
	f = fopen("testfiles/f.txt", "r");
	len = flen_safe(f);
	test(len == 13);
	fclose(f);
}

void test_flen_fseek()
{
	FILE *f = NULL;
	size_t len = 0;
	f = fopen("testfiles/f.txt", "r");
	len = flen_fseek(f);
	test(len == 13);
	fclose(f);
}

void test_flen()
{
	test_flen_safe();
	test_flen_fseek();
	FILE *f = NULL;
	size_t len = 0;
	f = fopen("testfiles/f.txt", "r");
	len = flen(f);
	test(len == 13);
	fclose(f);
}
/* }}} */

void test_fget()
{
	char *result = (char *)fget("testfiles/f.txt");
	const char *expected = "this is file\n";
	test(!strcmp(expected, result));
	free(result);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "f");
	test_flen();
	test_fget();
	return EXIT_SUCCESS;
}
