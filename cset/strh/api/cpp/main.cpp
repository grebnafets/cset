#define SAFEFORK_HOOK
#include <cset/safefork/safefork.h>
#include <cset/test/test.h>
#include <cset/strh/strh.h>
#include <cset/strh/api/cpp/strh.cpp.h>

void test_strh_create()
{
	Strh result = Strh("foobar");
	const char *expected = "foobar";
	test(!strcmp(expected, (char *)result.get()));
}

void test_strh_equ()
{
	Strh dis = Strh("foo");
	const char *foo = "foo";
	const char *bar = "bar";
	test(dis.equ(foo));
	test(!dis.equ(bar));
}

void test_strh_chk()
{
	Strh foobar = Strh("foobarfoobar");
	test(foobar.chk("bar", 3));
	test(foobar.equ(foobar.get()));
}

void test_strh_indexOf()
{
	Strh haystack = Strh("1a3b5c7d9eyぁ");
	size_t *one     = haystack.indexOf("a", 0);
	size_t *three   = haystack.indexOf("b", 1);
	size_t *five    = haystack.indexOf("c", 2);
	size_t *seven   = haystack.indexOf("d", 0);
	size_t *nine    = haystack.indexOf("e", 4);
	size_t *eleven  = haystack.indexOf("ぁ", 0);
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
	Strh foobar = Strh("foo, there are three foo here foobar smurf\n");
	size_t *foo = foobar.indexOf("foo", 0);
	test(*foo == 3);
	test(*(foo+1) EQU 0);
	test(*(foo+2) EQU 21);
	test(*(foo+3) EQU 30);
	free(foo);

	foo = foobar.indexOf("foo", 2);
	test(*foo EQU 2);
	test(*(foo+1) EQU 0);
	test(*(foo+2) EQU 21);
	free(foo);
}

void test_strh_utf8()
{
	
	test(Strh::utf8::w("ぁ") EQU 3);
	test(!bad);
	test(Strh::utf8::len("ぁぁぁぁ") EQU 4);
	test(Strh::utf8::len("ぁぁぁぁぁぁぁ") EQU 7);
	test(Strh::utf8::len("ぁぁ ぁぁぁ ぁぁ") EQU 9);
	Strh foo = Strh("ぁぁ ぁぁぁ ぁぁ");
	test(foo.utf8len() EQU 9);
}

void test_strh_put()
{
	Strh foo = Strh("Hello !");
	foo.put("world", 6);
	test(foo.equ("Hello world!"));
	foo.put(", we meet again", 11);
	test(foo.equ("Hello world, we meet again!"));
	foo.put("again, ", 0);
	test(foo.equ("again, Hello world, we meet again!"));
	foo.put("again", foo.bytelen());
	test(foo.equ("again, Hello world, we meet again!again"));
	foo.put("not inserted", 200);
	test(bad);
	test(is(STRH_INVALID_STR_LOCATION));
	statenoreset();
	foo.put("!", 5);
	test(foo.equ("again!, Hello world, we meet again!again"));
}

void test_combo()
{
	Strh foobar = Strh("foobar");
	size_t *i = foobar.indexOf("bar", 0);
	foobar.put("ぁ", *(i+1));
	test(*i == 1);
	test(*(i+1) == 3);
	test(foobar.equ("fooぁbar"));
	free(i);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "strh");
	test_strh_create();
	test_strh_equ();
	test_strh_chk();
	test_strh_utf8();
	test_strh_indexOf();
	test_strh_put();
	test_combo();
	return EXIT_SUCCESS;
}
