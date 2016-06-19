#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define CSET_GATE_PRE printf("%s\n", __func__);
//#define CSET_GATE_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/gate/gate.h>

#ifdef __unix__
#include <unistd.h>
#include <pthread.h>
#endif
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

void exampleUse(int count, cset_gate_Pass pass)
{
#ifdef __unix__
	sleep(1);
#endif
	int underflow;
	printf("pass=%d, gate=%d\n", pass, gate);
	cset_gate_Enter(&gate, &pass);
	if (count == 3) {
		// pass = 0; // force deadlock.
	}
	if (count < 5) {
		printf("Count = %d\n", count);
		exampleUse(++count, pass);
	}
	underflow = cset_gate_Leave(&gate, &pass);
	if (!underflow) {
		printf("pass=%d, gate=%d\n", pass, gate);
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

#ifdef __unix__
volatile int running_threads = 0;
pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid[2];
void *thread(void *arg)
{
	cset_gate_Pass pass = 0;
	exampleUse(0, pass);
	pthread_mutex_lock(&running_mutex);
	running_threads--;
	pthread_mutex_unlock(&running_mutex);
}
#endif

int main(int argc, char **argv)
{
	int i, err;
/*	cset_gate_Pass pass = 0;
	one();
	two();
	exampleUse(0, pass);
	bar();
	*/
#ifdef __unix__
	for (i = 0; i < 2; i += 1) {
		err = pthread_create(&(tid[i]), NULL, &thread, i?&err:0);
		if (err) {
			printf("error thread: (%s)\n", strerror(err));
		}
		pthread_mutex_lock(&running_mutex);
		running_threads++;
		pthread_mutex_unlock(&running_mutex);
	}
	while(running_threads > 0) {
		sleep(1);
	}
#endif
	return EXIT_SUCCESS;
}
