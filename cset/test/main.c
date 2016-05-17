#include <stdio.h>
#include <stdlib.h>
//#define CSET_TEST_PRE printf("%s\n", __func__);
//#define CSET_TEST_POST printf("%s\n", __func__);
#include <cset/test/test.h>

// Example use.

void foo() {
	struct cset_test_Data *foo = NULL;
	char *err = NULL;
	err = cset_test_New(err, "foo", CSET_TEST_MODE_SHOW_SHEBANG, &foo);
	err = test(err, 1 == 1, "One equals one", foo);
	err = test(err, 1 == 2, "One equals two", foo);
	err = cset_test_SetFGSuccess(err, 10, 255, 10, 0, foo);
	err = cset_test_SetBGSuccess(err, 100, 10, 100, 0, foo);
	err = cset_test_SetFGFailure(err, 255, 10, 10, 0, foo);
	err = cset_test_SetBGFailure(err, 10, 100, 100, 0, foo);
	err = cset_test_Display(err, foo);
	err = cset_test_Fini(err, &foo);
	if (err != NULL) {
		fprintf(stderr, "%s\n", err);
	}
}

void bar() {
	struct cset_test_Data *bar = NULL;
	char *err = NULL;
	err = cset_test_New(err, "foo", CSET_TEST_MODE_SHOW_FAILURE, &bar);
	err = test(err, 1 == 1, "One equals one", bar);
	err = test(err, 1 == 2, "One equals two", bar);
	err = cset_test_SetFGFailure(err, 254, 254, 254, 0, bar);
	err = cset_test_Display(err, bar);
	err = cset_test_Fini(err, &bar);
	if (err != NULL) {
		fprintf(stderr, "%s\n", err);
	}
}

int main(int argc, char **argv)
{
	bar();
	foo();
	return EXIT_SUCCESS;
}
