#ifndef STRH_LINUX
#define STRH_LINUX 1

/* c START {{{ */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* }}} */

/*
 * Assumes UTF 8
 * */

#include <cset/badcontext/badcontext.h>
#include <cset/mem/mem.h>
#include <cset/charmap/charmap.h>

void *strh_create(const void *str)
{
	char *tmp = (char *)str;
	size_t size = strlen(tmp);
	char *ret = (char *)mem.xc(size + 1, sizeof(char));
	strcpy(ret, tmp);
	return (void *)ret;
}

int strh_equ(const void *str1, const void *str2)
{
	char *s1, *s2;
	s1 = (char *)str1;
	s2 = (char *)str2;
	return !strcmp(s1, s2);
}

int strh_chk(const void *str1, const void *str2)
{
	int res = 0;
	unsigned char *s1 = (unsigned char *)str1;
	unsigned char *s2 = (unsigned char *)str2;
	size_t i, s1len, s2len;
	s1len = strlen((char *)s1);
	s2len = strlen((char *)s2);
	if (s2len > s1len) {goto OUT;}
	for (i = 0; i < s2len; i += 1) {
		if (s1[i] != s2[i]) {goto OUT;}
	}
	res = 1;
OUT:
	return res;
}

size_t *strh_indexOf(const void *haystack, const void *needle, size_t at)
{
	size_t pre[256];
	size_t *res = NULL;
	unsigned char *h = (unsigned char *)haystack;
	unsigned char *n = (unsigned char *)needle;
	size_t i, j, len;
        len = strlen((char *)h);
	for (i = 0, j = 0; i < len; i += 1) {
		if (h[i] == n[0]) {
			if (strh_chk(&h[i], n)) {
				pre[j++] = i;
				if (at != 0) {
					if (j == at) {break;}
				}
			}
		}
	}
	res = (size_t *)mem.xm((j + 1) * sizeof(size_t));
	res[0] = j;
	for (i = 1; i < j + 1; i += 1) {
		res[i] = pre[i - 1];
	}
	return res;
}

const int STRH_INVALID_CHAR_BYTE_ORIGIN = __COUNTER__;

size_t strh_utf8_width(const void *ch)
{
	unsigned char *c = (unsigned char *)ch;
	size_t i = 0;
	if ((c[0] & 0xC0) == 0x80) {
		cntxt = STRH_INVALID_CHAR_BYTE_ORIGIN;
		bad = 1;
		/* Fall through intentional. Give warning and allow caller
		 * to decite what to do.
		 * */
	}
	while (((c[i + 1] & 0xC0) == 0x80)) {i++;}
	return i + 1;
}

size_t strh_utf8_len(const void *str)
{
	unsigned char *s = (unsigned char *)str;
	unsigned char *t = s;
	size_t i, j, w, len;
	w = 0;
	j = 0;
	len = strlen((char *)str);
	for (i = 0, j = 0; i < len; i += w, j += 1) {
		w = strh_utf8_width(t);
		t += w;
	}
	return j;
}

const int STRH_INVALID_STR_LOCATION = __COUNTER__;

void strh_insert_segfault()
{
	fprintf(stderr, "strh.insert(**, *)\n");
	fprintf(stderr, "            ^\n");
}

/*
 * Param of interest: size_t at
 *
 * 1) 0 means prepend object to subject,
 * 2) strlen(string) means append obejct to subject,
 * 3) Anything between injects the string.
 * */
void strh_put(void *subject, const void *object, size_t at)
{
	unsigned char **s = (unsigned char **)subject;
	unsigned char *o = (unsigned char *)object;
	/* Get subject and object length, expect segfault. */
	segfault_handle = &strh_insert_segfault;
	size_t slen = strlen((char *)*s);
	segfault_handle = NULL;
	size_t olen  = strlen((char *)o);
	size_t displ = 0;
	/* Check vality of insert location. */
	if (at > slen) {
		cntxt = STRH_INVALID_STR_LOCATION;
		bad = 1;
		goto OUT;
	}
	/* Calculte displacement. */
	displ = slen - at;
	/* Insert string. */
	if (olen > 0) {
		*s = (unsigned char *)mem.xr(
			*s,
			(slen + olen + 1) * sizeof(char)
		);
		memset(*s + slen, '\0', olen + 1);
		memmove(*s + at + olen, *s + at, displ);
		memmove(*s + at, o, olen);
	}
OUT:
	return;
}

#ifndef __cplusplus
struct strh {
	void *(*create)(const void *str);
	int (*equ)(const void *str1, const void *str2);
	int (*chk)(const void *str1, const void *str2);
	struct strh_utf8 {
		size_t (*w)(const void *ch);
		size_t (*len)(const void *str);
	} utf8;
	size_t *(*indexof)(const void *haystack, const void *needle, size_t at);
	void (*put)(void *subject, const void *object, size_t at);
} strh = {
	.create   = &strh_create,
	.equ      = &strh_equ,
	.chk      = &strh_chk,
	.utf8.w   = &strh_utf8_width,
	.utf8.len = &strh_utf8_len,
	.indexof  = &strh_indexOf,
	.put      = &strh_put
};
#endif /* __cplusplus */

/* c END {{{ */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/* }}} */

#endif /* STRH_LINUX */
