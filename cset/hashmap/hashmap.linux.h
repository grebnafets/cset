#ifndef HASHMAP_LINUX
#define HASHMAP_LINUX 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <cset/badcontext/badcontext.h>
#include <cset/mem/mem.h>
#include <string.h>
#include <pthread.h>

/* types {{{ */
#define HASHMAP_ARRAY (size_t)1
#define HASHMAP_LIST (size_t)2
/* }}} */

/* sizes {{{ */
#define HASHMAP_TINY (size_t)2
#define HASHMAP_128 (size_t) 128
#define HASHMAP_K1 (size_t)1 << 10
#define HASHMAP_K4 (size_t)1 << 12
#define HASHMAP_K8 (size_t)1 << 13
#define HASHMAP_K32 (size_t)1 << 15
#define HASHMAP_M1 (size_t)1 << 20
#define HASHMAP_M4 (size_t)1 << 22
#define HASHMAP_M8 (size_t)1 << 23
#define HASHMAP_M33 (size_t)1 << 25
/* }}} */

/* memory increase scalars {{{ */
#ifndef HASHMAP_FALLBACK_TRIGGER
#define HASHMAP_FALLBACK_TRIGGER 1<<10
#endif /* HASHMAP_FALLBACK_TRIGGER */

#ifndef HASHMAP_FALLBACK
#define HASHMAP_FALLBACK 100
#endif /* HASHMAP_FALLBACK */
/* }}} */

/* errors {{{ */
const int HASHMAP_INVALID_TYPE  = __COUNTER__;
const int HASHMAP_INVALID_SIZE  = __COUNTER__;
const int HASHMAP_KEY_EXISTS    = __COUNTER__;
const int HASHMAP_KEY_NOT_FOUND = __COUNTER__;
/* }}} */

/* checks {{{ */
size_t hashmap_increase_size(size_t size)
{
	size_t ret;
	if (size < (HASHMAP_FALLBACK_TRIGGER)) {
		ret = size << 1;
	} else {
		ret = size + HASHMAP_FALLBACK;
	}
	return ret;
}

int hashmap_is_size_valid(int size)
{
	/* Kudos for exploringbinary.com */
	return ((size > 0) && !(size & (size - 1)));
}
/* }}} */

/* data structures {{{ */
struct hashmap_array {
	char **key;
	void **data;
	size_t ptr;
	size_t len;
};

struct hashmap_list {
	char *key;
	void *data;
	struct hashmap_list *next;
};

typedef size_t (*HASHMAP_FUNCTION)(const void *key, size_t size);

struct hashmap {
	void **data;
	size_t len;
	size_t type;
	size_t collision;
	HASHMAP_FUNCTION hash;
};
/* }}} */

/* default hash function {{{ */
/*
 * I am not a mathematician so I don't really know how I can approach this
 * better. Alas, more tests need to be added before making further experiments.
 * */
#ifndef HASHMAP_HASH
#define HASHMAP_HASH
size_t hashmap_hash(const void *key, size_t size)
{
	char *k;
	size_t ret, i, len, tmp;
	k   = (char *)key;
	/* ---Multiply first and the last and add to strlen ---*/
	tmp = len = strlen(k);
	tmp += k[0] * k[len - 1];
	/* ---Distribute around the center of the allocated space. --- */
	ret = size / 2;
	if ((tmp & 2) == 2) {
		ret += tmp;	
	} else {
		ret -= tmp;
	}
	/* ----------------------------------------------------------- */
	for (i = 1; i < len - 1; i += 1) {
		ret += k[i];
	}
	return (ret & (size - 1));
}
#endif /* HASHMAP_HASH */
/* }}} */

/* hashmap_create {{{ */
struct hashmap *hashmap_create(size_t size, HASHMAP_FUNCTION hash, size_t type)
{
	size_t i;
	struct hashmap *map;
       	i = 0;
	map = NULL;
	switch (type) {
	case HASHMAP_ARRAY: break;	
	case HASHMAP_LIST: break;
	default:
		bad = 1;
		cntxt = HASHMAP_INVALID_TYPE;
		goto OUT;
	}
	if (!hashmap_is_size_valid(size)) {
		bad = 1;
		cntxt = HASHMAP_INVALID_SIZE;
		goto OUT;
	}
	map = (struct hashmap *)mem.xm(sizeof(struct hashmap));
	map->hash = hash;
	map->len  = size;
	map->type = type;
	map->collision = 0;
	map->data = (void **)mem.xm(size * sizeof(void *));
	for (i = 0; i < size; i += 1) {
		map->data[i] = NULL;
	} 
OUT:
	return map;
}
/* }}} */

/* cleanup {{{ */

void hashmap_free_array(struct hashmap *map)
{
	size_t i, j, leni, lenj;
	struct hashmap_array *bucket;
	leni = map->len;
	for (i = 0; i < leni; i += 1) {
		bucket = (struct hashmap_array *)map->data[i];
		if (bucket != NULL) {
			lenj = bucket->ptr;
			for (j = 0; j < lenj; j += 1) {
				free(bucket->key[j]);
			}
			free(bucket->key);
			free(bucket->data);
			free(bucket);
		}
	}
	free(map->data);
	free(map);
}

void hashmap_free_list_nodes(struct hashmap_list *list)
{
	if (list->next != NULL) {
		hashmap_free_list_nodes(list->next);
	}
	free(list->key);
	free(list);
}

void hashmap_free_list(struct hashmap *map)
{
	size_t i, len;
	struct hashmap_list *list;
	len = map->len;
	for (i = 0; i < len; i += 1) {
		if (map->data[i] != NULL) {
			list = (struct hashmap_list *)map->data[i];
			hashmap_free_list_nodes(list);
		}
	}
	free(map->data);
	free(map);
}

void hashmap_free(struct hashmap *map)
{
	size_t type;
	type = map->type;
	switch (type) {
	case HASHMAP_ARRAY:
		hashmap_free_array(map);
		break;
	case HASHMAP_LIST:
		hashmap_free_list(map);
		break;
	}
}

void hashmap_del_array(struct hashmap *map, const void *key)
{
	size_t i, len, hash;
	struct hashmap_array *bucket = NULL;
	char *k;
	k = (char *)key;
	if (map->hash != NULL) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	bucket = (struct hashmap_array *)map->data[hash];
	if (bucket != NULL) {
		len = bucket->ptr;
		for (i = 0; i < len; i += 1) {
			if (bucket->key[i] != NULL) {
				if (!strcmp(bucket->key[i], k)) {
					free(bucket->key[i]);
					bucket->key[i]  = NULL;
					bucket->data[i] = NULL;
					break;
				}
			}
		}
	}
}

void hashmap_del_list(struct hashmap *map, const void *key)
{
	size_t hash;
	struct hashmap_list *pre, *post, *list;	
	char *k;
	k = (char *)key;
	if (map->hash != NULL) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	pre = post = NULL;
	list = (struct hashmap_list *)map->data[hash];
	if (list != NULL) {
		while (1) {
			if (list == NULL) {
				break;
			} else if (list->next != NULL) {
				post = list->next;
			}
			if (!strcmp(list->key, k)) {
				free(list->key);
				free(list);
				if (pre != NULL && post != NULL) {
					pre->next = post;
				} else if (pre != NULL) {
					pre->next = NULL;
				} else if (post != NULL) {
					map->data[hash] = post;
				} else {
					map->data[hash] = NULL;
				}
				break;
			}
			pre  = list; 
			list = list->next;
		}
	}
}

void hashmap_del(struct hashmap *map, const void *key)
{
	size_t type;
	type = map->type;
	switch (type) {
	case HASHMAP_ARRAY:
		hashmap_del_array(map, key);
		break;
	 case HASHMAP_LIST:
		hashmap_del_list(map, key);
		break;
	}
}
/* }}} */

/* put {{{ */
/* array {{{ */
void hashmap_put_array(struct hashmap *map, const void *key, void *val)
{
	char *k;
	size_t index, hash, i, len, empty, emptyi;
	struct hashmap_array *bucket = NULL;
	empty = 0;
	k = (char *)key;
	/* Get hash {{{ */
	if (map->hash != NULL) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	/* }}} */
	bucket  = (struct hashmap_array *)map->data[hash];
	if (bucket == NULL) {
		/* Init bucket {{{ */
		bucket = (struct hashmap_array *)mem.xm(
			sizeof(struct hashmap_array)
		);
		bucket->data = NULL;
		bucket->ptr = 0;
		bucket->len = 1;
		bucket->data = (void **)mem.xm(1 * sizeof(void *));
		bucket->key  = (char **)mem.xm(1 * sizeof(char *));
		map->data[hash] = (void *)bucket;
		/* }}} */
	} else {
		/* Check if key exists and log empty {{{ */
		len = bucket->ptr;
		for (i = 0; i < len; i += 1) {
			if (bucket->key[i] == NULL) {
				empty  = 1;
				emptyi = i;	
			} else if (!strcmp(bucket->key[i], k)) {
				cntxt = HASHMAP_KEY_EXISTS;
				goto OUT;
			}
		}
		/* }}} */
		map->collision++;
	}
	if (empty) {
		index = emptyi;
	} else {
		index = bucket->ptr++;
	}
	if (bucket->ptr == bucket->len) {
		/* Increase size {{{ */
		bucket->len  = hashmap_increase_size(bucket->len);
		bucket->data = (void **)mem.xr(
			bucket->data, bucket->len * sizeof(void *)
		);
		bucket->key  = (char **)mem.xr(
			bucket->key, bucket->len * sizeof(char *)
		);
		for (i = bucket->ptr; i < bucket->len; i += 1) {
			bucket->data[i] = NULL;
			bucket->key[i]  = NULL;
		}
		/* }}} */
	}
	/* Insert {{{ */
	bucket->key[index] = (char *)mem.xc(strlen(k) + 1, sizeof(char));
	strcpy(bucket->key[index], k);
	bucket->data[index] = val;
	/* }}} */
OUT:
	return;
}
/* }}} */

/* list {{{ */
void hashmap_put_list(struct hashmap *map, const void *key, void *val)
{
	char *k;
	size_t hash;
	struct hashmap_list *list, *tmp;
	k = (char *)key;
	/* Get hash {{{ */
	if (map->hash != NULL) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	/* }}} */
	list = (struct hashmap_list *)map->data[hash];
	if (list == NULL) {
		/* Insert node. {{{ */
		list = (struct hashmap_list *)mem.xm(
			sizeof(struct hashmap_list)
		);
		list->key = (char *)mem.xc(strlen(k) + 1, sizeof(char));
		strcpy(list->key, k);
		list->data = val;
		list->next = NULL;
		map->data[hash] = (void *)list;
		/* }}} */
		goto OUT;
	}
	while (1) {
		if (!strcmp(list->key, k)) {
			cntxt = HASHMAP_KEY_EXISTS;
			goto OUT;
		}
		if (list->next == NULL) {
			tmp = list;
			list = list->next;
			/* Insert node. {{{ */
			list = (struct hashmap_list *)mem.xm(sizeof(struct hashmap_list));
			list->key = (char *)mem.xc(strlen(k) + 1, sizeof(char));
			strcpy(list->key, k);
			list->data = val;
			list->next = NULL;
			/* }}} */
			tmp->next = list;
			goto OUT;
		}
		list = list->next;
	}
	map->collision++;
OUT:
	return;
}
/* }}} */

void hashmap_put(struct hashmap *map, const void *key, void *val)
{
	size_t type;
	type = map->type;
	switch (type) {
	case HASHMAP_ARRAY:
		hashmap_put_array(map, key, val);
		break;
	case HASHMAP_LIST:
		hashmap_put_list(map, key, val);
		break;
	}
}
/* }}} */

/* get {{{ */
/* array {{{ */
void *hashmap_get_array(struct hashmap *map, const void *key)
{
	void *ret, **data;
	char *k, **keys;
	size_t hash, i, len;
	struct hashmap_array *bucket = NULL;
	k = (char *)key;
	ret = NULL;
	if (map->hash) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	if (map->data[hash] != NULL) {
		bucket = (struct hashmap_array *)map->data[hash];
		keys = bucket->key;
		data = bucket->data;
		len = bucket->ptr;
		for (i = 0; i < len; i += 1) {
			if (keys[i] != NULL) {
				if (!strcmp(k, keys[i])) {
					ret = data[i];
					break;
				}
			}
		}
	}
	if (ret == NULL) {
		cntxt = HASHMAP_KEY_NOT_FOUND;
	}
	return ret;
}
/* }}} */


void *hashmap_get_list(struct hashmap *map, const void *key)
{
	char *k;
	size_t hash;
	struct hashmap_list *list;
	void *ret = NULL;
	k = (char *)key;
	/* Get hash {{{ */
	if (map->hash != NULL) {
		hash = map->hash(k, map->len);
	} else {
		hash = hashmap_hash(k, map->len);
	}
	/* }}} */
	list = (struct hashmap_list *)map->data[hash];
	while (1) {
		if (list == NULL) {
			break;
		}
		if (!strcmp(list->key, k)) {
			ret = list->data;
			break;
		}
		list = list->next;
	}
	if (ret == NULL) {
		cntxt = HASHMAP_KEY_NOT_FOUND;
	}
	return ret;
}

void *hashmap_get(struct hashmap *map, const void *key)
{
	size_t type;
	void *ret;
	type = map->type;
	switch (type) {
	case HASHMAP_ARRAY:
		ret = hashmap_get_array(map, key);
		break;
	case HASHMAP_LIST:
		ret = hashmap_get_list(map, key);
		break;
	}
	return ret;
}
/* }}} */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* HASHMAP_LINUX */
