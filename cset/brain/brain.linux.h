#ifndef BRAIN_LINUX
#define BRAIN_LINUX 1

#include <cset/mem/mem.h>
struct neuron {
	void *data;
	struct neuron **link;
	unsigned char *path;
	size_t connected;
};

struct neuron *brain_create_neuron()
{
	struct neuron *n = (struct neuron *)mem.xm(sizeof(struct neuron));
	n->connected = 0;
	n->path = NULL;
	n->link = NULL;
	n->data = NULL;
	return n;
}

struct neuron *brain_ping_neuron(struct neuron *n, unsigned char path)
{
	struct neuron *ret = NULL;
	long index = -1;
	size_t i, len;
	len = n->connected;
	for (i = 0; i < len; i += 1) {
		if (n->path[i] == path) {
			index = (long)i;
			break;
		}
	}
	if (index != -1) {
		ret = n->link[index];
	}
	return ret;
}

struct neuron *brain_find_neuron(struct neuron *n, unsigned char path)
{
	long index = -1;
	size_t i, len;
	len = n->connected;
	for (i = 0; i < len; i += 1) {
		if (n->path[i] == path) {
			index = (long)i;
			break;
		}
	}
	if (index == -1) {
		index = n->connected++;
		n->path = (char *)mem.xr(
			n->path, n->connected * sizeof(char)
		);
		n->link = (struct neuron **)mem.xr(
			n->link, n->connected * sizeof(struct neuron *)
		);
		n->path[index] = path;
		n->link[index] = brain_create_neuron();
	}
	return n->link[index];
}

struct neuron *brain_get_neuron(struct neuron *brain, const void *path)
{
	unsigned char *p = (unsigned char *)path;
	struct neuron *link = brain;
	size_t i, len;
        len = strlen((char *)p);
	for (i = 0; i < len; i += 1) {
		link = brain_find_neuron(link, p[i]);
	}
	return link;
}

void brain_free(struct neuron *brain)
{
	size_t number_of_neurons;
	number_of_neurons = brain->connected;
	while (number_of_neurons--) {
		brain_free(brain->link[number_of_neurons]);
	}
	if (brain->path != NULL) {
		free(brain->path);
	}
	if (brain->link != NULL) {
		free(brain->link);
	}
	free(brain);
}

#endif /* BRAIN_LINUX */
