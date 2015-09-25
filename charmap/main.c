#include "../test/test.h"

struct charmap_hook {
	unsigned char *key;
	void *val;
	size_t parent;
	size_t len;
	size_t level;
	unsigned char *indices;
	struct charmap **down;
};

void print_charmap(void *ret, const int ch)
{
	struct charmap_hook *map = (struct charmap_hook *)ret;
	long i, len;
	long found = -1;
	long index;
	len = (long)map->len;
	for (i = 0; i < len; i += 1) {
		fprintf(stdout, "[%c]", (char)map->indices[i]);
		if (map->indices[i] == (unsigned char)ch) {
			found = i;
			index = i;
		}
	}
	fprintf(stdout, "\n");
	if (found == -1) return;
	len = len * 3;
	found = found * 3 + 1;
	for (i = 0; i < len; i += 1) {
		if (i != found) {
			fprintf(stdout, " ");
		} else {
			fprintf(stdout, "%s", TS_ARROW_DOWN);
			break;
		}
	}
	fprintf(stdout, "\n");
	for (i = 0; i < len; i += 1) {
		if (i != found) {
			fprintf(stdout, " ");
		} else {
			fprintf(stdout, "%c", (char)map->indices[index]);
			break;
		}
	}
	fprintf(stdout, "\n");
}

#define CHARMAP_FIND_HOOK(ret, ch) print_charmap(ret, ch)

#include "charmap.h"

void test_charmap()
{
	size_t i, len;
	const char *keys[] = {
		"kone",
		"ktwo",
		"kthree",
		"kfour"
	};
	const char *vals[] = {
		"vone",
		"vtwo",
		"vthree",
		"vfour"
	};

	struct charmap *map = charmap_init();
	len = 4;
	for (i = 0; i < len; i += 1) {
		charmap_insert(map, keys[i], (void *)vals[i]);
	}
	struct charmap *found;
	for (i = 0; i < len; i += 1) {
		found = charmap_find(map, keys[i]);
		test(!strcmp((char *)found->val, (char *)vals[i]));
		test(!strcmp((char *)found->key, (char *)keys[i]));
	}

	charmap_free(map);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "charmap");
	test_charmap();
}
