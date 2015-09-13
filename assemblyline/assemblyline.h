#ifndef ASSEMBLYLINE
#define ASSEMBLYLINE

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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

void __assemblyline_push(struct assemblyline *line, size_t count, ...)
{
	size_t currpos, i;
	va_list args;
	ASSEMBLY_UNIT unit;
	va_start(args, count);
        currpos = line->len;
	line->len += count;
	line->unit = mem.xr(line->unit, line->len * sizeof(ASSEMBLY_UNIT));
	for (i = currpos; i < line->len; i += 1) {
		unit = va_arg(args, ASSEMBLY_UNIT);
		line->unit[i] = unit;
	}
	va_end(args);
}
#define ASSEMBLY_UNIT_NUMARGS(args...) ((size_t) sizeof((ASSEMBLY_UNIT[]){args}) / sizeof(ASSEMBLY_UNIT))
#define assemblyline_push(line, args...) __assemblyline_push(line, ASSEMBLY_UNIT_NUMARGS(args), args)

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

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* ASSEMBLYLINE */
