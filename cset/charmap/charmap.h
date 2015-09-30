#ifndef CHARMAP
#define CHARMAP 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <cset/mem/mem.h>

/* TODO: Make c++ abstraction. */

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#ifndef CHARMAP_FIND_HOOK
#define CHARMAP_FIND_HOOK(ret, ch)
#endif /* CHARMAP_FIND_HOOK */

struct charmap {
	unsigned char *key;
	void *val;
	size_t parent;
	size_t len;
	size_t level;
	unsigned char *indices;
	struct charmap **down;
};

struct charmap *charmap_init()
{
	struct charmap *map = NULL;
	map = (struct charmap *)mem.xm(sizeof(struct charmap));
	map->key     = NULL;
	map->val     = NULL;
	map->indices = NULL;
	map->down    = NULL;
	map->parent  = 0;
	map->len     = 0;
	map->level   = 0;
	return map;
}

struct charmap *charmap_down(struct charmap *map, const int ch)
{
	long index = -1;
	size_t i, len;
	len = map->len;
	struct charmap *ret = NULL;
	for (i = 0; i < len; i += 1) {
		if (map->indices[i] == ch) {
			index = (long)i;
			break;
		}
	}
	if (index != -1) {
		ret = map->down[index];
	}
	return ret;
}

struct charmap *charmap_down_insert(struct charmap *map, const int ch)
{
	int index;
	struct charmap *ret = NULL;
	ret = charmap_down(map, ch);
	if (ret == NULL) {
		index = map->len++;
		map->indices = (unsigned char *)mem.xr(
			map->indices, map->len * sizeof(char)
		);
		map->down = (struct charmap **)mem.xr(
			map->down, map->len * sizeof(struct charmap *)
		);
		map->indices[index] = (unsigned char)ch;
		map->down[index] = charmap_init();
		ret = map->down[index];
		ret->parent = index;
	}
	return ret;
}

struct charmap *charmap_insert(struct charmap *map, const void *key, void *val)
{
	struct charmap *ret = map;
	unsigned char *k = (unsigned char *)key;
	size_t i, len;
	int ch;
	len = strlen((char *)k);
	for (i = 0; i < len; i += 1) {
		ch = (int)k[i];
		ret = charmap_down_insert(ret, ch);
	}
	ret->key = k;
	ret->val = val;
	return ret;
}

struct charmap *charmap_find(struct charmap *map, const void *key)
{
	struct charmap *ret = map;
	unsigned char *k = (unsigned char *)key;
	size_t i, len;
	int ch;
	len = strlen((char *)k);
	for (i = 0; i < len; i += 1) {
		ch = (int)k[i];
		CHARMAP_FIND_HOOK(ret, ch);
		ret = charmap_down(ret, ch);
		if (ret == NULL) {
			break;
		}
	}
	return ret;
}

void charmap_free(struct charmap *map)
{
	size_t len;
	len = map->len;
	while (len--) {
		charmap_free(map->down[len]);
	}
	if (map->indices != NULL) {
		free(map->indices);
	}
	if (map->down != NULL) {
		free(map->down);
	}
	free(map);
}

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* CHARMAP */
