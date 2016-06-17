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
// // Double ugh...

// TODO: Write test that forces spinlock and releace form a different thread.

cset_gate_Gate gate = 0;
cset_gate_Pass pass = 0; // Here pass is shared between two functions.

void one()
{
	printf("one pass=%d, gate=%d\n", pass, gate);
	cset_gate_Enter(&gate, &pass);
	cset_gate_Leave(&gate, &pass);
	printf("one pass=%d, gate=%d\n", pass, gate);
}

void two()
{
	printf("two pass=%d\n", pass, gate);
	cset_gate_Enter(&gate, &pass);
	one();
	cset_gate_Leave(&gate, &pass);
	printf("two pass=%d\n", pass, gate);
}

void exampleUse(int count)
{
	int underflow;
	static cset_gate_Pass myStaticPass = 0;
	printf("pass=%d, gate=%d\n", myStaticPass, gate);
	cset_gate_Enter(&gate, &myStaticPass);
	if (count == 3) {
		// myStaticPass = 0; // force deadlock.
	}
	if (count < 5) {
		printf("Count = %d\n", count);
		exampleUse(++count);
	}
	underflow = cset_gate_Leave(&gate, &myStaticPass);
	if (!underflow) {
		printf("pass=%d, gate=%d\n", myStaticPass, gate);
	} else {
		printf("underflow detected\n");
	}
}

void foo()
{
	printf("foo gate=%d\n", gate);
	cset_gate_Lock(&gate);
	cset_gate_Unlock(&gate);
	printf("foo gate=%d\n", gate);

}

void bar()
{
	printf("bar gate=%d\n", gate);
	foo();
	cset_gate_Lock(&gate);
	// foo(); // force deadlock
	cset_gate_Unlock(&gate);
	printf("bar gate=%d\n", gate);
}

int main(int argc, char **argv)
{
	one();
	two();
	exampleUse(0);
	bar();
	return EXIT_SUCCESS;
}
