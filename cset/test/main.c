#include <stdlib.h>
#include <cset/test/test.h>

int main(int argc, char **argv)
{
	struct cset_test_Data *testData = NULL;
	char *err = NULL;
	err = cset_test_New(err, "foo", 7, __FILE__, &testData);
	printf("%d\n", err);
	printf("%s:%s:%d\n", testData->name, testData->filename, testData->mode);
	test(err, 1 == 1, "foobar", testData);
	if (err != NULL) {
		printf("%s\n", err);
	}
	printf("test data total == %d\n", testData->total);
	printf("test data success == %d\n", testData->success);
//	printf("test case desc: %s\n", testData->cases[0]->description);
	err = cset_test_Fini(err, &testData);
	if (testData == NULL) {
		printf("t is NULL\n");
	}
	if (err != NULL) {
		printf("%s\n", err);
	}
	return EXIT_SUCCESS;
}
