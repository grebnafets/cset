/*
 * There are few ways to do parsing. From dynamically allocating stack for a
 * thread before using recursions, to state machines.
 * Both are easy to make at first but they become really hard maintain as they
 * grow.
 * 
 * Is there another way? Maybe, I am still exploring the possibility myself.
 * It wouldn't surprise me if another superior way sprung up from below.
 *
 * I believe the hardest part of parsing is using sentence diagrams.
 * To explain what I mean, I am going to use a very simple analogy:
 * If you print out x*2=y from x=1 to x=10, you get a nice table. The "table"
 * is the sentence diagram but the formula "y=x*2" is essence of what you
 * really need before parsing. Even harder still, there are usually many
 * formulas you need to follow before you can parse anything.
 * I don't mean sentence diagrams are bad, they just come naturally after.
 * Before sentence diagrams, you need simple and normal sets to contain all the
 * formulas you need.
 *
 * You can either use Venn or Eular diagram. I would argue that Eular is
 * better because it is simpler.
 * Venn creates amiguity with unions, so we try our best to be smart and
 * creative to remove them. This leaves us with nice and simple Eular diagram.
 * With Eular diagram, we can with ease create the first step before parsing.
 *
 * The same problem reveals itself when we start to think in terms of events
 * and states. It is really simpler to think in terms of keywords and how they
 * map within a set.
 *
 * Now this sounds way too simple and it is easier said than done or done
 * before said...
 *
 * What about AST, tokenization, lookaheads and the rest of the
 * complex clusterfuck? And don't we already have libraries that abstract this
 * clusterfuck away for us?
 *
 * It is indeed hard and impossible if you only think about about making a
 * program instead of making a simple structure. I claim that all we need is
 * one simple structure.
 *
 * The structure I currently use to solve this problem is really simple but
 * also really slow in nature. Many people filter this possibility out because
 * it is in essence a dynamic linked list that looks horrible on paper,
 * especially when it comes to big O(n) notion with respect to dereferencing a
 * cache missed pointer.
 *
 * In practice however, this is bottleneck is going to happen anyway. It is
 * also ironic because in a way, the power of OOP languages come from the use
 * of the this structure. It is not as clear, but objects are basically
 * structs/classes that can link to network of other structs/classes. AST is
 * also a linked list structure and you are going to need recursion to thread
 * over it after you created it.
 *
 * With the neuron struct, I have abstracted the essential part I need to
 * make the keyset. I will be able to tell the difference between "do" and
 * "done" without any special effort or lookahead condition. The look ahead
 * is simply automatic and doesn't require any states. As long as the keys
 * don't match, I can map them. I can also map the "formula" of the states
 * they might carry along with them.
 *
 * While the struct is the essential part, there is bit more to this.
 * Another important step is about local and global management of memory using
 * stacks and hashmaps. I will hopefully give myself time to explain that part
 * elsewhere.
 *
 * */

#ifndef EULARPARSE_LINUX
#define EULARPARSE_LINUX 1

#include <cset/mem/mem.h>
#include <cset/strh/strh.h>
#include <stdarg.h>
/* Neuron struct is found here. */
#include <cset/brain/brain.h>

/* eularparse set mask {{{ */
#define EP_MASK_EN_ALPHA_SMALL (size_t)1
#define EP_MASK_EN_ALPHA_BIG (size_t)2
#define EP_MASK_NUMBER (size_t)4
void __eularparse_set_mask(unsigned char *mask, size_t argcount, ...)
{
	size_t i;
	va_list argptr;
	int on;
	size_t flags;
	va_start(argptr, argcount);
	flags = va_arg(argptr, size_t);
	memset(mask, 0, 256);
	if ((flags & EP_MASK_EN_ALPHA_BIG) == EP_MASK_EN_ALPHA_BIG) {
		memset(mask+65, 1, 26);
	}
	if ((flags & EP_MASK_EN_ALPHA_SMALL) == EP_MASK_EN_ALPHA_SMALL) {
		memset(mask+97, 1, 26);
	}
	if ((flags & EP_MASK_NUMBER) == EP_MASK_NUMBER) {
		memset(mask+48, 1, 10);
	}
	for (i = 1; i < argcount; i += 1) {
		on = va_arg(argptr, size_t);
		mask[on] = 1;
	}	
}
#define eularparse_set_mask(m, args...) __eularparse_set_mask(m, (sizeof((size_t []){args}) / sizeof(size_t)), args)
/* }}} */

/* structs {{{  */
struct eularparse_info {
	size_t type;
	size_t *subtype;
	size_t subtypecount;
};

struct eularparse_stack {
	size_t n;
	unsigned char *name;
	unsigned char *base;
	struct eularparse_info *info;
};

struct eularparse {
	struct eularparse_s {
		struct eularparse_stack *data;
		size_t sptr;
	} stack;
	struct eularparse_i {
		struct eularparse_info *list;
		size_t len;
		size_t current;
	} info;
	unsigned char *sep;
	struct neuron *brain;
};
/* }}} */

/* new eularparse info {{{ */
size_t new_eularparse_info(struct eularparse *e, size_t type)
{
	size_t index = e->info.len++;
	e->info.list = (struct eularparse_info *)mem.xr(
		e->info.list,
		e->info.len * sizeof(struct eularparse_info)
	);
	e->info.list[index].type = type;
	e->info.list[index].subtype = NULL;
	e->info.list[index].subtypecount = 0;
	return index;
}
/* }}} */

/* eularparse info add subtype {{{ */
void eularparse_add_subtype(struct eularparse *e, size_t index, size_t type)
{
	size_t sindex = e->info.list[index].subtypecount++;
	e->info.list[index].subtype = (size_t *)mem.xr(
		e->info.list[index].subtype,
		e->info.list[index].subtypecount * sizeof(size_t)
	);
	e->info.list[index].subtype[sindex] = type;
}
/* }}} */

/* eularparse_link_info {{{  */
void eularparse_link_info(struct eularparse *e, const void *association, size_t info)
{
	struct neuron *link = NULL;
	link = brain_get_neuron(e->brain, association);
	link->data = (void *)info;
}
/* }}} */

/* eularparse_link_info {{{  */
void eularparse_link_info_mask(
	struct eularparse *e, const void *mask, size_t info
)
{
	unsigned char *m = (unsigned char *)mask;
	size_t i, len;
	struct neuron *link = NULL;
	unsigned char buf[2] = {'\0'};
	len = 256;
	for (i = 0; i < len; i += 1) {
		if (m[i]) {
			memset(buf, '\0', 2);
			sprintf(buf, "%c", i);
			link = brain_get_neuron(e->brain, buf);
			link->data = (void *)info;
		}
	}
}
/* }}} */

/* eularparse get index {{{ */
long eularparse_get_info_index(
	struct eularparse *e, const void *str, size_t pos
)
{
	size_t i = pos;
	unsigned char *s = (unsigned char *)str;
	long info = -1;
	struct neuron *o, *n;
        n = o = e->brain;
	while(s[i] != '\0') {
		o = n;
		n = brain_ping_neuron(o, s[i++]);
		if (n == NULL) {
			break;
		}
	}
	if (s[i] == '\0' && n != NULL) {
		o = n;
	}
	if (o != NULL && o != e->brain) {
		info = (size_t)o->data;
	}
	return info;
}
/* }}} */

/* get info by index {{{ */
struct eularparse_info *eularparse_get_current_info(struct eularparse *e)
{
	size_t info_index = e->info.current; 
	struct eularparse_info *info = &(e->info.list[info_index]);
	return info;
}
/* }}} */

/* eularparse get current id {{{ */
size_t eularparse_get_current_id(struct eularparse *e)
{
	size_t n = e->stack.data[e->stack.sptr - 1].n - 1;
	return n;
}
/* }}} */

/* eularparse get base {{{ */
unsigned char *eularparse_get_base(struct eularparse *e)
{
	return e->stack.data[e->stack.sptr - 1].base;
}
/* }}} */

/* {{{ */
unsigned char *eularparse_get_name(struct eularparse *e)
{
	return e->stack.data[e->stack.sptr - 1].name;
}
/* }}} */

/* new_eularparse {{{ */
struct eularparse *new_eularparse(const void *sep)
{
	struct eularparse *e = (struct eularparse *)mem.xm(sizeof(struct eularparse));
	e->stack.data = NULL;
	e->stack.sptr = 0;
	e->info.list = NULL;
	e->info.len = 0;
	e->brain = brain_create_neuron();
	e->sep = strh_create(sep);
	return e;
}
/* }}} */

/* deostroy eularparse info {{{ */
void destroy_eularparse_info(struct eularparse *e)
{
	size_t i, len;
	len = e->info.len;
	for (i = 0; i < len; i += 1) {
		free(e->info.list[i].subtype);
	}
	free(e->info.list);
}
/* }}} */

/* destroy eularparse {{{ */
void destroy_eularparse(struct eularparse *e)
{
	size_t i, len;
	len = e->stack.sptr;
	for (i = 0; i < len; i += 1) {
		free(e->stack.data[i].name);
		free(e->stack.data[i].base);
	}
	free(e->stack.data);
	destroy_eularparse_info(e);
	free(e->sep);
	brain_free(e->brain);
	free(e);
}
/* }}} */

/* push {{{ */
#define EP_NEW 1
void eularparse_push(struct eularparse *e, const void *name, size_t isnew)
{
	unsigned char *n = (unsigned char *)name;
	size_t index = e->stack.sptr++;
	size_t size = 0;
	struct eularparse_stack enew;
	struct eularparse_stack eold;
	e->stack.data = (struct eularparse_stack *)mem.xr(
		e->stack.data,
		e->stack.sptr * sizeof(struct eularparse_stack)		
	);
	enew = e->stack.data[index];
	if (!isnew && index != 0) {
		eold = e->stack.data[index - 1];
		size = strlen((char *)eold.name);
		size += strlen(n);
		size += strlen((char *)e->sep);
		size += 1;
		enew.name = mem.xc(size, sizeof(char));
		strcpy((char *)enew.name, (char *)eold.name);
		strcat((char *)enew.name, e->sep);
		strcat((char *)enew.name, (char *)n);
		enew.base = strh_create(enew.name);
	} else {
		enew.name = strh_create(n);
		enew.base = strh_create(n);
	}
	enew.info = NULL;
	enew.n = 0;
	e->stack.data[index] = enew;
}
/* }}} */

/* pop {{{ */
struct eularparse_stack eularparse_pop(struct eularparse *e)
{
	struct eularparse_stack ret;
	size_t index = 0;
	if (e->stack.sptr > 1) {
		index = (--e->stack.sptr) - 1;
		ret = e->stack.data[index];
	} else if (e->stack.sptr > 0) {
		e->stack.sptr -= 1;
	} else {
		goto OUT;
	}
	free(e->stack.data[e->stack.sptr].name);
	free(e->stack.data[e->stack.sptr].base);
	if (e->stack.sptr) {
		e->stack.data = (struct eularparse_stack *)mem.xr(
			e->stack.data,
			e->stack.sptr * sizeof(struct eularparse_stack)		
		);
	} else {
		free(e->stack.data);
		e->stack.data = NULL;
	}
OUT:
	return ret;
}
/* }}} */

/* eularparse increment {{{ */
void eularparse_increment(struct eularparse *e)
{
	struct eularparse_stack *top = &(e->stack.data[e->stack.sptr - 1]);
	size_t n, nlen, blen, seplen;
	char id[32] = {'\0'};
	n = top->n++;
	sprintf(id, "%lu", n);
	free(top->name);
	nlen = strlen(id);
	blen = strlen((char *)top->base);
	seplen = strlen(e->sep);
	top->name = (unsigned char *)mem.xc(
		(blen + seplen + nlen + 1), sizeof(char)
	);
	strcpy(top->name, top->base);
	strcat(top->name, e->sep);
	strcat(top->name, id);
}
/* }}} */

#endif /* EULARPARSE_LINUX */
