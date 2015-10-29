#include <cset/test/test.h>
#include <cset/strh/strh.h>
#include <cset/brain/brain.h>

void test_brain()
{
	struct neuron *brain = brain_create_neuron();
	struct neuron *n1, *n2, *n3;
	n1 = brain_get_neuron(brain, "path");
	unsigned char *d1, *d2;
        d1 = strh_create("foo");
	d2 = strh_create("bar");
	n1->data = (void *)d1;
	n2 = brain_get_neuron(brain, "pa");
	n2->data = (void *)d2; 
	n3 = brain_get_neuron(brain, "path");
	test(strh.equ(d1, n3->data));
	n3 = brain_get_neuron(brain, "pa");
	test(strh.equ(d2, n3->data));
	free(d1);
	free(d2);
	brain_free(brain);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "brain");
	test_brain();
	return EXIT_SUCCESS;
}
