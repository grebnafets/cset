#include "../test/test.h"
#include "assemblyline.h"

struct dummy {
	size_t t1;
	size_t t2;
	size_t t3;		
};

void *fn1(void *arg)
{
	struct dummy *d = (struct dummy *)arg;
	d->t1 = 1;
	return (void *)d;
}

void *fn2(void *arg)
{
	struct dummy *d = (struct dummy *)arg;
	d->t2 = 2;
	return (void *)d;
}

void *fn3(void *arg)
{
	struct dummy *d = (struct dummy *)arg;
	d->t3 = 3;
	return (void *)d;
}

void test_assemblyline()
{
	struct dummy *d = mem.m(sizeof(struct dummy));
	d->t1 = 0;d->t2 = 0;d->t3 = 0;
	struct assemblyline *line = new_assemblyline((void *)d);
	assemblyline_push(line, &fn1);
	assemblyline_push(line, &fn2);
	assemblyline_push(line, &fn3);
	assemblyline_run(line);
	test(d->t1 == 1);
	test(d->t2 == 2);
	test(d->t3 == 3);
	d->t3 = 4;
	ASSEMBLY_UNIT unit = assemblyline_pop(line);
	test(d->t3 = 4);
	d = unit(d);
	test(d->t3 = 3);
	free_assemblyline(line);
	free(d);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);
	test_assemblyline();
}
