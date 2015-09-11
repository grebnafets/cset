#ifndef ASSEMBLYLINE
#define ASSEMBLYLINE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../mem/mem.h"

typedef void *(*ASSEMBLY_UNIT)(void *arg);

struct assemblyline {
	ASSEMBLY_UNIT *unit;
	void *arg;
	size_t len;
};

struct assemblyline *new_assemblyline(void *arg)
{
	struct assemblyline *line = NULL;
	line = mem.xm(sizeof(struct assemblyline));
	line->unit = NULL;
	line->arg  = arg;
	line->len  = 0;
	return line;
}

void assemblyline_push(struct assemblyline *line, ASSEMBLY_UNIT unit)
{
	size_t i = line->len++;
	line->unit = mem.xr(line->unit, line->len * sizeof(ASSEMBLY_UNIT));
	line->unit[i] = unit;
}

ASSEMBLY_UNIT assemblyline_pop(struct assemblyline *line)
{
	size_t i = --line->len;
	ASSEMBLY_UNIT unit = line->unit[i];
	line->unit[i] = NULL;
	line->unit = mem.xr(line->unit, line->len * sizeof(ASSEMBLY_UNIT));
	return unit;
}

void *assemblyline_run(struct assemblyline *line)
{
	size_t i, len;
	void *arg = line->arg;
	len = line->len;
	for (i = 0; i < len; i += 1) {
		arg = line->unit[i](arg);
	}
	return arg;
}

void free_assemblyline(struct assemblyline *line)
{
	free(line->unit);
	free(line);
}

#endif /* ASSEMBLYLINE */
