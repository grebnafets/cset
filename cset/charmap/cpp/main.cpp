#include <cset/test/test.h>
#include <cset/charmap/charmap.h>

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

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

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "charmap");
	test_charmap();
}
