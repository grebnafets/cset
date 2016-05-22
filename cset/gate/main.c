#include <stdio.h>
#include <stdlib.h>
//#define CSET_GATE_PRE printf("%s\n", __func__);
//#define CSET_GATE_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/gate/gate.h>

// Having only example here will have to do for now.
//
// It is going to be very hard to test for race conditions in userland
// environment. There is only one I know about and that is to force complete
// sync between two threads and force a race condition. In userland, this
// requires me to implement a fine grated virtual machine where I force
// spinlock on each turn. Ugh...

// TODO: Write test that forces spinlock and releace form a different thread.

cset_gate_Gate gate = 0;
cset_gate_Pass pass = 0; // Here pass is shared between two functions.

void one()
{
	printf("one pass=%d\n", pass);
	cset_gate_Enter(&gate, &pass);
	cset_gate_Leave(&gate, &pass);
	printf("one pass=%d\n", pass);
}

void two()
{
	printf("two pass=%d\n", pass);
	cset_gate_Enter(&gate, &pass);
	one();
	cset_gate_Leave(&gate, &pass);
	printf("two pass=%d\n", pass);
}

void exampleUse(int count)
{
	printf("pass=%d\n", pass);
	cset_gate_Enter(&gate, &pass);
	if (count == 3) {
		// pass = 0; // force deadlock.
	}
	if (count < 5) {
		printf("Count = %d\n", count);
		exampleUse(++count);
	}
	cset_gate_Leave(&gate, &pass);
	printf("pass=%d\n", pass);
}

int main(int argc, char **argv)
{
	one();
	two();
	exampleUse(0);
	return EXIT_SUCCESS;
}
