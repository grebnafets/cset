/* WARNING. "safefork" Does not mean it is safe to fork within threads.
 * I did try and I am open for possible solutions, but right now it is simply
 * asking for a trouble. You can try it yourself here, for me I had invalid
 * read that I could not solve or fully reason why. You will need a
 * comprehensive low level understanding to explain why and even so, there
 * might not be a solution at hand.
 *
 * Maybe it is easier to write a lesser degree of fork, something like a
 * thread copy or something... But that is a philosophy for a another time...
 *
 * What I see as "safe" is having the error correction tools happy. Feel free
 * to update me if they are not happy on your machine. I can only check for
 * happines on my machine.
 * */

/* Testing fork safty is hard, so I basically cheated by checking if
 * spinlock did run and if the program exited normally. (That means no
 * spinlock.)
 * Sooo...
 * 	step 1. Make forks and nrees run longer with SAFEFORK_SPIN.
 * 	step 2. In thread A -> run bunch of reallocs.
 * 	step 3. Run bunch of threads.
 * 	step 4. Hope both spinlocks will run.....
 * 	step 5. Stand up and turn around 2 times.
 * 	step 6. Sit down and check the results. If the program is still
 * 		running, then you have a spinlock. Patch it, then stand up
 * 		again and spin twice but now in the other direction.
 *
 * If you have a better approach then feel free to write over this mess.
 * */

long ffork_wait = 0;
long fnree_wait = 0;
#ifndef SAFEFORK_HOOK
	#define SAFEFORK_HOOK
#endif /* SAFEFORK_HOOK */
#define NREE_SPIN fnree_wait++
#define FORK_SPIN ffork_wait++
#define SAFEFORK_SPIN usleep(1)

#include <cset/safefork/safefork.h>

#define TEST_SILENT_IF_NO_TESTS
#include <cset/test/test.h>

pthread_t t;

void *thread(void *arg)
{
	size_t i, len;
	void *ptr = NULL;
	len = 5;
	for (i = 0; i < len; i += 1) {
		ptr = realloc(ptr, i);
		usleep(1);
	}
	free(ptr);
	return arg;
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "safefork");
	pid_t mainpid = getpid();
	pthread_create(&t, NULL, &thread, NULL);
	/* Create forks here. */
	size_t len;
	len = 5;
	while (len--) {fork(); usleep(1);}
	if (bad) {
		fprintf(stderr, "error@fork_cleanup\n");
		abort();
	}
	pthread_join(t, NULL);
	fork_cleanup();

	/* If these values are more then 0, then at some point there was a 
	 * lock but it didn't end in a spinlock. */
	if (getpid() == mainpid) {
		test(ffork_wait > 0);
		test(fnree_wait > 0);
	}
	return EXIT_SUCCESS;
}
