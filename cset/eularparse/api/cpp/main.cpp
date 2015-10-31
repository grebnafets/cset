#include <cset/test/test.h>
#include <cset/eularparse/eularparse.h>
#include <cset/assemblyline/assemblyline.h>

void test_eularparse_push_pop()
{
	struct eularparse *e = new_eularparse("/");
	eularparse_push(e, "foo", 0);
	eularparse_push(e, "bar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo/bar"));
	struct eularparse_stack stack = eularparse_pop(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo"));
	test(!strcmp((char *)stack.name, "foo"));
	eularparse_push(e, "foobar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo/foobar"));
	eularparse_push(e, "bar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo/foobar/bar"));
	eularparse_push(e, "hello", 1);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello"));
	eularparse_push(e, "world", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello/world"));
	destroy_eularparse(e);

	e = new_eularparse("///");
	eularparse_push(e, "foo", 0);
	eularparse_push(e, "bar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo///bar"));
	stack = eularparse_pop(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo"));
	test(!strcmp((char *)stack.name, "foo"));
	eularparse_push(e, "foobar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo///foobar"));
	eularparse_push(e, "bar", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "foo///foobar///bar"));
	eularparse_push(e, "hello", 1);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello"));
	eularparse_push(e, "world", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world"));
	eularparse_increment(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world///0"));
	eularparse_increment(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world///1"));
	eularparse_push(e, "we meet again!", 0);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world///1///we meet again!"));
	eularparse_increment(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world///1///we meet again!///0"));
	stack = eularparse_pop(e);
	test(!strcmp((char *)e->stack.data[e->stack.sptr - 1].name, "hello///world///1"));
	destroy_eularparse(e);
}

void test_eularparse_info()
{
	struct eularparse *e = new_eularparse("/");
	size_t one = new_eularparse_info(e, 1);
	size_t two = new_eularparse_info(e, 2);
	eularparse_add_subtype(e, one, 1);
	eularparse_add_subtype(e, one, 2);
	eularparse_add_subtype(e, two, 3);
	eularparse_add_subtype(e, two, 4);
	eularparse_link_info(e, "foo", one);
	eularparse_link_info(e, "bar", two);
	size_t ione = (size_t)brain_get_neuron(e->brain, "foo")->data;
	size_t itwo = (size_t)brain_get_neuron(e->brain, "bar")->data;
	test(e->info.list[ione].type == 1);
	test(e->info.list[itwo].type == 2);
	test(e->info.list[ione].subtype[0] == 1);
	test(e->info.list[ione].subtype[1] == 2);
	test(e->info.list[itwo].subtype[0] == 3);
	test(e->info.list[itwo].subtype[1] == 4);
	destroy_eularparse(e);
}

/* test mask {{{ */
void test_eularparse_mask()
{
	unsigned char cmp_field_mask[256] = {0};
	cmp_field_mask['0'] = 1;
	cmp_field_mask['1'] = 1;
	cmp_field_mask['2'] = 1;
	cmp_field_mask['3'] = 1;
	cmp_field_mask['4'] = 1;
	cmp_field_mask['5'] = 1;
	cmp_field_mask['6'] = 1;
	cmp_field_mask['7'] = 1;
	cmp_field_mask['8'] = 1;
	cmp_field_mask['9'] = 1;

	cmp_field_mask['a'] = 1;
	cmp_field_mask['b'] = 1;
	cmp_field_mask['c'] = 1;
	cmp_field_mask['d'] = 1;
	cmp_field_mask['e'] = 1;
	cmp_field_mask['f'] = 1;
	cmp_field_mask['g'] = 1;
	cmp_field_mask['h'] = 1;
	cmp_field_mask['i'] = 1;
	cmp_field_mask['j'] = 1;
	cmp_field_mask['k'] = 1;
	cmp_field_mask['l'] = 1;
	cmp_field_mask['m'] = 1;
	cmp_field_mask['n'] = 1;
	cmp_field_mask['o'] = 1;
	cmp_field_mask['p'] = 1;
	cmp_field_mask['q'] = 1;
	cmp_field_mask['r'] = 1;
	cmp_field_mask['s'] = 1;
	cmp_field_mask['t'] = 1;
	cmp_field_mask['u'] = 1;
	cmp_field_mask['v'] = 1;
	cmp_field_mask['w'] = 1;
	cmp_field_mask['x'] = 1;
	cmp_field_mask['y'] = 1;
	cmp_field_mask['z'] = 1;

	cmp_field_mask['A'] = 1;
	cmp_field_mask['B'] = 1;
	cmp_field_mask['C'] = 1;
	cmp_field_mask['D'] = 1;
	cmp_field_mask['E'] = 1;
	cmp_field_mask['F'] = 1;
	cmp_field_mask['G'] = 1;
	cmp_field_mask['H'] = 1;
	cmp_field_mask['I'] = 1;
	cmp_field_mask['J'] = 1;
	cmp_field_mask['K'] = 1;
	cmp_field_mask['L'] = 1;
	cmp_field_mask['M'] = 1;
	cmp_field_mask['N'] = 1;
	cmp_field_mask['O'] = 1;
	cmp_field_mask['P'] = 1;
	cmp_field_mask['Q'] = 1;
	cmp_field_mask['R'] = 1;
	cmp_field_mask['S'] = 1;
	cmp_field_mask['T'] = 1;
	cmp_field_mask['U'] = 1;
	cmp_field_mask['V'] = 1;
	cmp_field_mask['W'] = 1;
	cmp_field_mask['X'] = 1;
	cmp_field_mask['Y'] = 1;
	cmp_field_mask['Z'] = 1;

	cmp_field_mask['_'] = 1;
	cmp_field_mask['-'] = 1;
	cmp_field_mask[10] = 1;

	unsigned char *field_mask = (unsigned char *)mem.xc(256, sizeof(char));
	eularparse_set_mask(
		field_mask,
		0
		| EP_MASK_EN_ALPHA_SMALL
		| EP_MASK_EN_ALPHA_BIG
		| EP_MASK_NUMBER,
		'_', '-', 10
	);
	size_t i, len;
	len = 256;
	for (i = 0; i < len; i += 1) {
		if (cmp_field_mask[i] == field_mask[i]) {
			if (test_mode_is(TEST_SHOW_SUCCESS)) {
				fprintf(stdout, "%3lu", i);
			}
		} else if (cmp_field_mask[i] != field_mask[i]) {
			if (test_mode_is(TEST_SHOW_FAILURE)) {
				fprintf(stdout, "%3lu", i);
			}
		}
		test(cmp_field_mask[i] == field_mask[i]);
	}
	free(field_mask);
}
/* }}} */


/* demo use {{{ */

/* 
 * Below is just written without any planing nor effort. It is still
 * tedious and dull to write a parser, but it is isn't that hard with Eular
 * approach and the barrier of entry is far lower with the bonus that scaling
 * it up is also the same simple tedious thing. Instead of being hard, it is
 * just boring.
 *
 * Eularparse is supposed to take away the need to think too hard about
 * parsing while having few limitations.
 *
 * Eularparse is not without faults. One of its main faults is the fact that
 * it is painfully slow.
 * The speed must be gained by strategy of not doing the same thing twice.
 * One example of such strategy would be to use it for preprocessing purposes
 * and push parsing behind version control within the build pipleline.
 * This would allow version control to signal what needs to be parsed and what
 * doesn't need to be parsed.
 *
 * Example: git status | myparser
 *
 * Steps needed.
 * 1) define tokens
 * 2) map tokens
 * 3) create and link handlers for the tokens
 * 4) do whatever
 * 5) cleanup
 *
 * */

/*
 * To make things a bit more interesting, we define field token that is
 * a-zA-Z_ character set. We also define do and done to see how easy it
 * to tell the diffirence between the two without states.
 * */

/* Our tokens */
#define OBJ 1
#define ARR 2
#define FIELD 3
#define NUMBER 4
#define COLON 5
#define COMMA 6
#define DO 7
#define DONE 8

/* Here we define the position of the subset we want to use. */
#define EMPTY0 0
#define AT 0
#define NUMBERTYPE 0

/* OBJ[AT] and ARR[AT] */
/* BEGIN and END signal weather token signals beginning or end. */	
#define BEGIN 1
#define END 2

/* NUMBER[NUMBERTYPE] = EMPTY0/FLOAT */
#define FLOAT 1

/* Our parse container. */
struct demo_parse {
	size_t pos;
	unsigned char *ch;
	struct eularparse *ep;
	/* Lets overkill it and make each handle as assemblyline. */
	struct assemblyline *obj;
	struct assemblyline *arr;
	struct assemblyline *field;
	struct assemblyline *comma;
	struct assemblyline *colon;
	struct assemblyline *number;
	struct assemblyline *_do;
	struct assemblyline *_done;
};

/* Our handles. No need to think hard, just assume you have the right token
 * and fart through it. */
/* handles {{{ */
void *handle_obj_start(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	struct eularparse_info *info = eularparse_get_current_info(p->ep);
	unsigned char *c = p->ch;
	unsigned char *objname;
	size_t pos = p->pos;
	size_t obj_starts_at = pos;
	if (info->subtype[AT] == BEGIN) {
		eularparse_push(p->ep, "obj", 0);
		eularparse_increment(p->ep);
		test(obj_starts_at == 0);
		objname = eularparse_get_name(p->ep);
		test(!strcmp((char *)objname, "obj/0"));
	}
	return (void *)p;
}

void *handle_obj_end(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	struct eularparse_info *info = eularparse_get_current_info(p->ep);
	unsigned char *c = p->ch;
	size_t pos = p->pos;
	size_t obj_ends_at = pos;
	if (info->subtype[AT] == END) {
		eularparse_pop(p->ep);
		test(obj_ends_at == 22);
	}
	return (void *)p;

}

void *handle_arr_start(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	struct eularparse_info *info = eularparse_get_current_info(p->ep);
	unsigned char *c = p->ch;
	unsigned char *arrname;
	size_t pos = p->pos;
	size_t arr_starts_at = pos;
	if (info->subtype[AT] == BEGIN) {
		eularparse_push(p->ep, "arr", 0);
		eularparse_increment(p->ep);
		test(arr_starts_at == 11);
		arrname = eularparse_get_name(p->ep);
		test(!strcmp((char *)arrname, "obj/0/arr/0"));
	}
	return (void *)p;
}

void *handle_arr_end(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	struct eularparse_info *info = eularparse_get_current_info(p->ep);
	unsigned char *c = p->ch;
	unsigned char *arrname;
	size_t pos = p->pos;
	size_t arr_ends_at = pos;
	if (info->subtype[AT] == END) {
		eularparse_pop(p->ep);
		test(arr_ends_at == 21);
		arrname = eularparse_get_name(p->ep);
		test(!strcmp((char *)arrname, "obj/0"));
	}
	return (void *)p;
}

void *handle_comma(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	size_t n = eularparse_get_current_id(p->ep);
	unsigned char *name = eularparse_get_name(p->ep);
	char str[128] = "obj/0/arr/";
	char top[32] = {'\0'};
	sprintf(top, "%lu", n);
	strcat(str, top);
	test(!strcmp((char *)name, (char *)str));
	eularparse_increment(p->ep);
	return (void *)p;
}

void *handle_colon(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	size_t position_of_colon = p->pos;
	test(position_of_colon == 10);
	/* Here you might replace key with value. */
	return (void *)p;
}

void *handle_number(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	size_t n = eularparse_get_current_id(p->ep);
	struct eularparse_info *info = eularparse_get_current_info(p->ep);
	unsigned char *str = p->ch;
	size_t pos = p->pos;
	size_t numberat = pos;
	size_t isfloat = 0;
	while ((str[pos] >= '0' && str[pos] <= '9') || str[pos] == '.') {
		if (str[pos] == '.' && !isfloat) {
			isfloat = 1;
			info->subtype[NUMBERTYPE] = FLOAT;	
		} else if (str[pos] == '.' && isfloat) {
			/* Simple error */
			break;
		}
		pos++;
	}
	if (n == 0) {
		test(n == 0 && numberat == 12);
	} else if (n == 1) {
		test(n == 1 && numberat == 15);
	} else if (n == 2) {
		test(n == 2 && numberat == 18);
		test(info->subtype[NUMBERTYPE] == FLOAT);
	}
	p->pos = pos - 1;
}

void *handle_field(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	unsigned char *c = p->ch;
	size_t pos = p->pos;
	size_t field_found_at_location = pos;
	test(field_found_at_location == 1);
	unsigned char mask[256] = {0};
	eularparse_set_mask(
		mask,
		0
		| EP_MASK_EN_ALPHA_SMALL
		| EP_MASK_EN_ALPHA_BIG
		| EP_MASK_NUMBER,
		'_'
	);
	unsigned char cmp[32] = {'\0'};
	unsigned char tmp[2] = {'\0'};
	while (mask[c[pos]]) {
		memset(tmp, '\0', 2);
		tmp[0] = c[pos];
		strcat((char *)cmp, (char *)tmp);
		pos++;
	}
	p->pos = pos - 1;
	size_t field_done_at_location = pos - 1;
	test(field_done_at_location == 9);
	test(!strcmp((char *)cmp, "field_123"));
	/* 
	 * Here it is possible to set field to value so it can be handled
	 * later.
	 * Field would be stored without a base but strings would be stored
	 * with a base to differentiate between local and global via
	 * hashmap.
	 * Example:
	 * {
	 * 	{
	 *		foo:"whatever",
	 *		"bar": foo
	 * 	} 
	 * }
	 * "foo" would be stored as is while "bar" might be prefixed with
	 * something like obj/0/obj/0/ so it becomes obj/0/obj/0/bar
	 * */
	return (void *)p;
}

void *handle_do(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	p->pos += 1;
	size_t _do = 1;
	test(_do);
}

void *handle_done(void *arg)
{
	struct demo_parse *p = (struct demo_parse *)arg;
	p->pos += 3;
	size_t done = 1;
	test(done);
}
/* }}} */

/* handle router. Again, no need to think. */
void handle(struct demo_parse *p)
{
	size_t info_index = p->ep->info.current; 
	struct eularparse_info info = p->ep->info.list[info_index];
	switch (info.type) {
	case OBJ:
		assemblyline_run(p->obj);
		break;
	case ARR:
		assemblyline_run(p->arr);
		break;
	case FIELD:
		assemblyline_run(p->field);
		break;
	case COLON:
		assemblyline_run(p->colon);
		break;
	case COMMA:
		assemblyline_run(p->comma);
		break;
	case NUMBER:
		assemblyline_run(p->number);
		break;
	case DO:
		assemblyline_run(p->_do);
		break;
	case DONE:
		assemblyline_run(p->_done);
		break;
	default:
		/* is space */
		break;
	}
}

void test_parsing()
{
	/* define tokens */
	struct eularparse *ep = new_eularparse("/");
	size_t obj_begin = new_eularparse_info(ep, OBJ);
	size_t obj_end   = new_eularparse_info(ep, OBJ);
	size_t arr_begin = new_eularparse_info(ep, ARR);
	size_t arr_end   = new_eularparse_info(ep, ARR);
	size_t field     = new_eularparse_info(ep, FIELD);
	size_t number    = new_eularparse_info(ep, NUMBER);
	size_t colon     = new_eularparse_info(ep, COLON);
	size_t comma     = new_eularparse_info(ep, COMMA);
	size_t _do       = new_eularparse_info(ep, DO);
	size_t _done     = new_eularparse_info(ep, DONE);
	eularparse_add_subtype(ep, obj_begin, BEGIN);
	eularparse_add_subtype(ep, obj_end, END);
	eularparse_add_subtype(ep, arr_begin, BEGIN);
	eularparse_add_subtype(ep, arr_end, END);
	eularparse_add_subtype(ep, number, EMPTY0);

	/* map tokens */
	unsigned char *field_mask = (unsigned char *)mem.xc(256, sizeof(char));
	eularparse_set_mask(
		field_mask,
		0
		| EP_MASK_EN_ALPHA_SMALL
		| EP_MASK_EN_ALPHA_BIG,
		'_'
	);
	unsigned char *number_mask = (unsigned char *)mem.xc(256, sizeof(char));
	eularparse_set_mask(
		number_mask,
		0
		| EP_MASK_NUMBER
	);
	eularparse_link_info_mask(ep, field_mask, field);
	eularparse_link_info_mask(ep, number_mask, number);
	eularparse_link_info(ep, "{", obj_begin);
	eularparse_link_info(ep, "}", obj_end);
	eularparse_link_info(ep, "[", arr_begin);
	eularparse_link_info(ep, "]", arr_end);
	eularparse_link_info(ep, ":", colon);
	eularparse_link_info(ep, ",", comma);
	eularparse_link_info(ep, "do", _do);
	eularparse_link_info(ep, "done", _done);

	/* create test string */
	char teststr[] = "{field_123:[1, 2, 3.5]}dodone";
	struct demo_parse *p = (struct demo_parse *)mem.xm(sizeof(struct demo_parse));
	p->ep = ep;
	p->ch = (unsigned char *)&teststr;
	p->pos = 0;

	/* register handlers */
	p->obj = new_assemblyline(p);
	p->arr = new_assemblyline(p);
	p->field = new_assemblyline(p);
	p->comma = new_assemblyline(p);
	p->colon = new_assemblyline(p);
	p->number = new_assemblyline(p);
	p->_do = new_assemblyline(p);
	p->_done = new_assemblyline(p);
	assemblyline_push(p->obj, &handle_obj_start, &handle_obj_end);
	assemblyline_push(p->arr, &handle_arr_start, &handle_arr_end);
	assemblyline_push(p->field, &handle_field);
	assemblyline_push(p->colon, &handle_colon);
	assemblyline_push(p->comma, &handle_comma);
	assemblyline_push(p->number, &handle_number);
	assemblyline_push(p->_do, &handle_do);
	assemblyline_push(p->_done, &handle_done);
	size_t i, len;
	len = strlen(teststr);
	long info;

	/* run */
	for (i = 0; i < len; i += 1) {
		p->pos = i;
		info = eularparse_get_info_index(ep, teststr, i);
		if (info != -1) {
			p->ep->info.current = (size_t)info;
			handle(p);
		}
		i = p->pos;
	}

	/* cleanup */
	destroy_eularparse(ep);
	free_assemblyline(p->obj);
	free_assemblyline(p->arr);
	free_assemblyline(p->comma);
	free_assemblyline(p->colon);
	free_assemblyline(p->field);
	free_assemblyline(p->number);
	free_assemblyline(p->_do);
	free_assemblyline(p->_done);
	free(number_mask);
	free(field_mask);
	free(p);
}
/* }}} */

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "Eular parse");
	test_eularparse_push_pop();
	test_eularparse_info();
	test_eularparse_mask();
	test_parsing();
	return EXIT_SUCCESS;
}
