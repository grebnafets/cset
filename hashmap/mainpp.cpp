#include <iostream>
#define TEST_SILENT_IF_NO_TESTS
#include "../test/test.h"
#include "hashmap.cpp.h"
#include "../threadwrapp/threadwrapp.h"

void *test_hashmap_array(void *arg __attribute__((unused)));
void *test_hashmap_list(void *arg __attribute__((unused)));

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

void *test_hashmap_array(void *arg __attribute__((unused)))
{
	Hashmap map = Hashmap();
	char *foo = (char *)"foo";
	char *bar = (char *)"bar";
	map.put("key1", foo);
	map.put("key2", bar);
	char *get1 = (char *)map.get("key1");
	char *get2 = (char *)map.get("key2");

	test(!strcmp(get1, "foo"));
	test(!strcmp(get2, "bar"));
	test(!bad);
	test(!cntxt);

	map.del("key1");
	get1 = (char *)map.get("key1");
	test(get1 == NULL);

	char *fail = (char *)map.get("foobar");
	test(fail == NULL);
	test(is(HASHMAP_KEY_NOT_FOUND));
	cntxt = 0;
	return NULL;
}

void *test_hashmap_list(void *arg __attribute__((unused)))
{
	Hashmap map = Hashmap();
	char *foo = (char *)"foo";
	char *bar = (char *)"bar";
	map.put("key1", foo);
	map.put("key2", bar);
	char *get1 = (char *)map.get("key1");
	char *get2 = (char *)map.get("key2");

	test(!strcmp(get1, "foo"));
	test(!strcmp(get2, "bar"));
	test(!bad);
	test(!cntxt);

	map.del("key1");
	get1 = (char *)map.get("key1");
	test(get1 == NULL);

	char *fail = (char *)map.get("foobar");
	test(fail == NULL);
	test(is(HASHMAP_KEY_NOT_FOUND));
	cntxt = 0;
	return NULL;
}
