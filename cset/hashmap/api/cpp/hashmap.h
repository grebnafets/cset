#ifndef HASHMAPAPI
#define HASHMAPAPI 1

#include <cset/hashmap/hashmap.h>

class Hashmap {
	private:
		struct hashmap *map = NULL;
	public:
		void put(const void *key, void *val);
		void *get(const void *key);
		void del(const void *key);
		void use(HASHMAP_FUNCTION hash);
		Hashmap(size_t size, HASHMAP_FUNCTION hash, size_t type);
		Hashmap(size_t size, size_t type);
		Hashmap(size_t size);
		Hashmap(void);

		~Hashmap(void);
};

void Hashmap::put(const void *key, void *val)
{
	hashmap_put(map, key, val);
}

void *Hashmap::get(const void *key)
{
	return hashmap_get(map, key);
}

void Hashmap::del(const void *key)
{
	hashmap_del(map, key);
}

void Hashmap::use(HASHMAP_FUNCTION hash)
{
	map->hash = hash;
}

Hashmap::Hashmap(size_t size, HASHMAP_FUNCTION hash, size_t type)
{
	map = (struct hashmap *)hashmap_create(size, hash, type);
}
Hashmap::Hashmap(size_t size, size_t type)
{
	map = (struct hashmap *)hashmap_create(size, NULL, type);
}
Hashmap::Hashmap(size_t size)
{
	map = (struct hashmap *)hashmap_create(size, NULL, HASHMAP_ARRAY);
}
Hashmap::Hashmap(void)
{
	map = (struct hashmap *)hashmap_create(HASHMAP_K1, NULL, HASHMAP_ARRAY);
}
Hashmap::~Hashmap(void)
{
	hashmap_free(map);
}

#endif /* HASHMAPAPI */
