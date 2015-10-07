/* TODO:
 * Testing hashmapping is more then just checking for bugs.
 * I think it is best to split results of distribution into separated
 * directory with local makefile.
 * I have no idea what to name that directory...
 * 	"research", "experiments", "data", "dance-of-the-penguins"...
 * 	no? I am out of ideas...
 * */

#define TEST_SILENT_IF_NO_TESTS
#include <cset/test/test.h>
#include <cset/hashmap/hashmap.h>
#include <cset/threadwrapp/threadwrapp.h>

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/* prepare {{{ */
void initdis() __attribute__((constructor));
void initdis()
{
	time_t t;
	srand((unsigned)time(&t));
}

#define SET "1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"
/* }}} */

/* utilities {{{ */
char *random_string(size_t len)
{
	size_t i;
	char set[] = SET;
	char *str = (char *)mem.xc(len + 1, sizeof(char));
	for (i = 0; i < len; i += 1) {
		str[i] = set[rand() % (sizeof(set) - 1)];
	}
	str[len] = '\0';
	return str;
}

struct pairs {
	char **keys;
	char **vals;
	size_t len;
};

struct pairs *make_pairs(size_t count, int random)
{
	struct pairs *p = (struct pairs *)mem.xm(sizeof(struct pairs));
	size_t i, len;
	len = 45;
	char vbuf[len];
	char kbuf[len];
	p->len = count;
	p->keys = (char **)mem.xm(count * sizeof(char *));
	p->vals = (char **)mem.xm(count * sizeof(char *));
	for (i = 0; i < count; i += 1) {
		memset(vbuf, '\0', len);
		sprintf(vbuf, "val%lu", i);
		p->vals[i] = (char *)mem.xc(len, sizeof(char));
		strcpy(p->vals[i], vbuf);
		if (random) {
			p->keys[i] = random_string(len);
		} else {
			memset(kbuf, '\0', len);
			sprintf(kbuf, "key%lu", i);
			p->keys[i] = (char *)mem.xc(len, sizeof(char));
			strcpy(p->keys[i], vbuf);
		}
	}
	return p;
}

void free_pairs(struct pairs *p)
{
	size_t i;
	for (i = 0; i < p->len; i += 1) {
		free(p->keys[i]);
		free(p->vals[i]);
	}
	free(p->keys);
	free(p->vals);
	free(p);
}
/* }}} */

void *test_hashmap_array1(void *arg __attribute__((unused)))
{
	size_t i, len;
	struct hashmap *map = hashmap_create(HASHMAP_TINY, NULL, HASHMAP_ARRAY);
	struct pairs *p = NULL;
	char *result;
	len = 100;
	p = make_pairs(len, 1);
	for (i = 0; i < len; i += 1) {
		//fprintf(stdout, "%s\n", p->keys[i]);
		hashmap_put(map, p->keys[i], p->vals[i]);
	}
	for (i = 0; i < len; i += 1) {
		result = (char *)hashmap_get(map, p->keys[i]);
		//fprintf(stdout, "%s\n", result);
		test(!strcmp(result, p->vals[i]));
	}	

	test(!bad);
	test(!cntxt);

	char *foo = (char *)"foo";
	char *bar = (char *)"bar";
	hashmap_put(map, "keyfoo", foo);
	hashmap_put(map, "keybar", bar);
	char *get1 = (char *)hashmap_get(map, "keyfoo");
	char *get2 = (char *)hashmap_get(map, "keybar");

	test(!strcmp(get1, "foo"));
	test(!strcmp(get2, "bar"));
	test(!bad);
	test(!cntxt);

	hashmap_del(map, "keyfoo");
	get1 = (char *)hashmap_get(map, "keyfoo");
	test(get1 == NULL);

	char *fail = (char *)hashmap_get(map, "foobar");
	test(fail == NULL);
	test(is(HASHMAP_KEY_NOT_FOUND));
	cntxt = 0;

	free_pairs(p);
	hashmap_free(map);
	return NULL;
}

void *test_hashmap_array2(void *arg __attribute__((unused)))
{
	size_t i, len;
	struct hashmap *map = hashmap_create(HASHMAP_128, NULL, HASHMAP_ARRAY);
	struct pairs *p = NULL;
	char *result;
	len = 100;
	p = make_pairs(len, 1);
	for (i = 0; i < len; i += 1) {
		//fprintf(stdout, "%s\n", p->keys[i]);
		hashmap_put(map, p->keys[i], p->vals[i]);
	}
	for (i = 0; i < len; i += 1) {
		result = (char *)hashmap_get(map, p->keys[i]);
		//fprintf(stdout, "%s\n", result);
		test(!strcmp(result, p->vals[i]));
	}	

	fprintf(stdout, "Random key collisions len=K1 : %lu\n", map->collision);
	free_pairs(p);
	hashmap_free(map);
	return NULL;
}

void *test_hashmap_array3(void *arg __attribute__((unused)))
{
	size_t i, len;
	struct hashmap *map = hashmap_create(HASHMAP_128, NULL, HASHMAP_ARRAY);
	struct pairs *p = NULL;
	char *result;
	len = 100;
	p = make_pairs(len, 0);
	for (i = 0; i < len; i += 1) {
		//fprintf(stdout, "%s\n", p->keys[i]);
		hashmap_put(map, p->keys[i], p->vals[i]);
	}
	for (i = 0; i < len; i += 1) {
		result = (char *)hashmap_get(map, p->keys[i]);
		//fprintf(stdout, "%s\n", result);
		test(!strcmp(result, p->vals[i]));
	}	

	fprintf(stdout, "Collisions len=K1 : %lu\n", map->collision);
	free_pairs(p);
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
	pthread_t t[4];
	pthread_create(&t[0], NULL, &test_hashmap_array1, NULL);
	pthread_create(&t[1], NULL, &test_hashmap_array2, NULL);
	pthread_create(&t[2], NULL, &test_hashmap_array3, NULL);
	pthread_create(&t[3], NULL, &test_hashmap_list, NULL);
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);
	pthread_join(t[2], NULL);
	pthread_join(t[3], NULL);
	return EXIT_SUCCESS;
}
