#define SAFEFORK_HOOK
#include <cset/safefork/safefork.h>
#include <cset/test/test.h>
#include <cset/strh/strh.h>

void test_strh_create()
{
	char *result = (char *)strh.create("foobar");
	const char *expected = "foobar";
	test(!strcmp(expected, result));
	free(result);
}

void test_strh_create_array()
{
	char **arr = (char **)strh_create_array(
		3,
		(char *[]){"one", "two", "three"}
	);
	test(strh.equ(arr[0], "one"));
	test(strh.equ(arr[1], "two"));
	test(strh.equ(arr[2], "three"));
	strh_free_array(3, arr);
}

void test_strh_equ()
{
	const char *foo = "foo";
	const char *bar = "bar";
	test(strh.equ(foo, foo));
	test(!strh.equ(foo, bar));
	const char *foobar = "foobarfoobar";
	test(!strh.equ(foobar + 3, "bar"));
}

void test_strh_chk()
{
	const char *foobar = "foobarfoobar";
	test(strh.chk(foobar + 3, "bar"));
}

void test_strh_indexOf()
{
	const char *haystack = "1a3b5c7d9eyぁ";
	size_t *one     = strh.indexof(haystack, "a", 0);
	size_t *three   = strh.indexof(haystack, "b", 1);
	size_t *five    = strh.indexof(haystack, "c", 2);
	size_t *seven   = strh.indexof(haystack, "d", 0);
	size_t *nine    = strh.indexof(haystack, "e", 4);
	size_t *eleven  = strh.indexof(haystack, "ぁ", 0);
	test(*one EQU 1);
	test(*(one+1) EQU 1);
	test(*three EQU 1);
	test(*(three+1) EQU 3);
	test(*five EQU 1);
	test(*(five+1) EQU 5);
	test(*seven EQU 1);
	test(*(seven+1) EQU 7);
	test(*nine EQU 1);
	test(*(nine+1) EQU 9);
	test(*eleven EQU 1);
	test(*(eleven+1) EQU 11);
	free(one);
	free(three);
	free(five);
	free(seven);
	free(nine);
	free(eleven);
	const char *foobar = "foo, there are three foo here foobar smurf\n";
	size_t *foo = strh.indexof(foobar, "foo", 0);
	test(*foo == 3);
	test(*(foo+1) EQU 0);
	test(*(foo+2) EQU 21);
	test(*(foo+3) EQU 30);
	free(foo);

	foo = strh.indexof(foobar, "foo", 2);
	test(*foo EQU 2);
	test(*(foo+1) EQU 0);
	test(*(foo+2) EQU 21);
	free(foo);
}

void test_strh_utf8()
{
	test(strh.utf8.w("ぁ") EQU 3);
	test(!bad);
	test(strh.utf8.len("ぁぁぁぁ") EQU 4);
	test(strh.utf8.len("ぁぁぁぁぁぁぁ") EQU 7);
	test(strh.utf8.len("ぁぁ ぁぁぁ ぁぁ") EQU 9);
}

void test_strh_put()
{
	char *foo = mem.xc(sizeof("Hello !") + 1, sizeof(char));
	strcpy(foo, "Hello !");
	strh.put(&foo, "world", 6);
	test(!strcmp((char *)foo, "Hello world!"));
	strh.put(&foo, ", we meet again", 11);
	test(!strcmp((char *)foo, "Hello world, we meet again!"));
	strh.put(&foo, "again, ", 0);
	test(!strcmp((char *)foo, "again, Hello world, we meet again!"));
	strh.put(&foo, "again", strlen(foo));
	test(!strcmp((char *)foo, "again, Hello world, we meet again!again"));
	strh.put(&foo, "not inserted", 200);
	test(bad);
	test(is(STRH_INVALID_STR_LOCATION));
	statenoreset();
	strh.put(&foo, "!", 5);
	test(!strcmp((char *)foo, "again!, Hello world, we meet again!again"));
	free(foo);
}

void test_combo()
{
	char *foobar = strh.create("foobar");
	size_t *i = strh.indexof(foobar, "bar", 0);
	strh.put(&foobar, "ぁ", *(i+1));
	test(*i == 1);
	test(*(i+1) == 3);
	test(strh.equ(foobar, "fooぁbar"));
	free(i);
	free(foobar);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "strh");
	test_strh_create();
	test_strh_create_array();
	test_strh_equ();
	test_strh_chk();
	test_strh_utf8();
	test_strh_indexOf();
	test_strh_put();
	test_combo();
	return EXIT_SUCCESS;
}
