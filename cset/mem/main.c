#include <stdio.h>
#include <stdlib.h>
//#define CSET_MEM_PRE printf("%s\n", __func__);
//#define CSET_MEM_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/mem/mem.h>

// TODO: test_malloc, test_calloc, test_realloc
void test_mem() {
	struct cset_test_Data *mem = NULL;
	char *err = NULL;
	err = cset_test_New(err, "malloc", CSET_TEST_MODE_SHOW_SHEBANG, &mem);
	err = cset_test_SetFGSuccess(err, 100, 255, 100, 0, mem);
	err = cset_test_SetFGFailure(err, 255, 10, 10, 0, mem);
	void *mal = NULL;
	err = cset_mem_malloc(err, 10, &mal);
	void *cal = NULL;
	err = cset_mem_calloc(err, 2, 10, &cal);
	err = cset_mem_realloc(err, 20, &mal);
	err = cset_mem_free(err, &mal);
	err = test(err, err == NULL, "was freed", mem);
	err = cset_mem_free(err, &cal);
	err = test(err, err == NULL, "was freed", mem);
	err = cset_mem_free(err, &cal);
	void *freed = strstr(err, "Cannot free NULL ptr");
	free(err);
	err = NULL;
	err = test(err, freed != NULL, "not freed", mem);
	err = test(err, mal == NULL, "is null", mem);
	err = test(err, cal == NULL, "is null", mem);
	err = cset_test_Display(err, mem);
	err = cset_test_Fini(err, &mem);
	if (err != NULL) {
		fprintf(stderr, "%s\n", err);
	}
}

int main(int argc, char **argv)
{
	test_mem();
	return EXIT_SUCCESS;
}
