#ifndef F
#define F

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cset/mem.h>

const int F_NULL_PARAM = __COUNTER__;

/* flen {{{ */
/* flen inner {{{ */
size_t flen_fseek(FILE *f)
{
	size_t len = 0;
	if (!f) {
		cntxt = F_NULL_PARAM;
		bad = 1;
		goto OUT;
	}
	fseek(f, 0, SEEK_END);
	if (ferror(f)) {cntxterrno(); goto OUT;}
	len = ftell(f);
	if (errno) {cntxterrno(); goto OUT;}
	fseek(f, 0, SEEK_SET);
	if (ferror(f)) {cntxterrno();}
OUT:
	return len;
}

size_t flen_safe(FILE *f)
{
	size_t len = 0;
	const size_t bufsize = 4096;
	char buf[bufsize];
	size_t tmp = 0;
	if (!f) {
		cntxt = F_NULL_PARAM;
		bad = 1;
		goto OUT;
	}
	while (1) {
		tmp = fread(buf, 1, bufsize, f);
		if (ferror(f)) {cntxterrno(); goto OUT;}
		len += tmp;
		if (tmp != bufsize) {
			break;
		}
	}
	rewind(f);
OUT:
	return len;
}
/* }}} */

/* flen main ii{{{ */
size_t flen(FILE *f)
{
	size_t len = 0;
	if (!f) {
		cntxt = F_NULL_PARAM;
		bad = 1;
		goto OUT;
	}
	len = flen_fseek(f);
	if (bad) {
		cntxtreset();
		len = flen_safe(f);
	}
OUT:
	return len;
}
/* }}} */
/* }}} */

void *fget(const char *filename)
{
	void *ret = NULL;
	FILE *f    = NULL;
	size_t len = 0;
	f = fopen(filename, "r");
	if (ferror(f)) {cntxterrno(); goto OUT;}
	len = flen(f);
	if (bad) {goto OUT;}
	ret = mem.xc(len + 1, sizeof(char));
	fread(ret, 1, len, f);
	if (ferror(f)) {cntxterrno();}
	fclose(f);
OUT:
	return ret;
}

void fput(const char *filename, const void *contents)
{
	char *c = (char *)contents;
	FILE *f = NULL;
	f = fopen(filename, "w");
	if (ferror(f)) {cntxterrno(); goto OUT;}
	fprintf(f, "%s", c);
	fclose(f);
OUT:
	return;
}

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* F */
