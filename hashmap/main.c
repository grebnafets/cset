#define TEST_SILENT_IF_NO_TESTS
#include "../test/test.h"
#include "hashmap.h"
#include "../threadwrapp/threadwrapp.h"

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

void *test_hashmap_array(void *arg __attribute__((unused)))
{
	struct hashmap *map = hashmap_create(HASHMAP_TINY, NULL, HASHMAP_ARRAY);
	char *foo = (char *)"foo";
	char *bar = (char *)"bar";
	hashmap_put(map, "key1", foo);
	hashmap_put(map, "key2", bar);
	char *get1 = (char *)hashmap_get(map, "key1");
	char *get2 = (char *)hashmap_get(map, "key2");

	test(!strcmp(get1, "foo"));
	test(!strcmp(get2, "bar"));
	test(!bad);
	test(!cntxt);

	hashmap_del(map, "key1");
	get1 = (char *)hashmap_get(map, "key1");
	test(get1 == NULL);

	char *fail = (char *)hashmap_get(map, "foobar");
	test(fail == NULL);
	test(is(HASHMAP_KEY_NOT_FOUND));
	cntxt = 0;

	hashmap_free(map);
	return NULL;
}

void *test_hashmap_list(void *arg __attribute__((unused)))
{
	struct hashmap *map = hashmap_create(HASHMAP_TINY, NULL, HASHMAP_LIST);
	char *foo = (char *)"foo";
	char *bar = (char *)"bar";
	hashmap_put(map, "key1", foo);
	hashmap_put(map, "key2", bar);
	char *get1 = (char *)hashmap_get(map, "key1");
	char *get2 = (char *)hashmap_get(map, "key2");

	test(!strcmp(get1, "foo"));
	test(!strcmp(get2, "bar"));
	test(!bad);
	test(!cntxt);

	hashmap_del(map, "key1");
	get1 = (char *)hashmap_get(map, "key1");
	test(get1 == NULL);

	char *fail = (char *)hashmap_get(map, "foobar");
	test(fail == NULL);
	test(is(HASHMAP_KEY_NOT_FOUND));
	cntxt = 0;

	hashmap_free(map);
	return NULL;
}

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "hashmap");
	pthread_t t[2];
	thr.create(&t[0], NULL, &test_hashmap_array, NULL);
	thr.create(&t[1], NULL, &test_hashmap_list, NULL);
	thr.join(t[0], NULL);
	thr.join(t[1], NULL);
	return EXIT_SUCCESS;
}
