#define TEST_SILENT_IF_NO_TESTS
#include "../test/test.h"

/* WARNING. Although fork is tested this way, you should be careful of forking
 * within a thread or within a fork. Not saying you can't or shouldn't, I am
 * just saying your error correction tools might bite you a bit later..
 * I know fork only copies page on write, still...
 * */

/* Testing fork safty is hard, so I basically cheated by checking if
 * spinlock did run and if the program exited normally. (That means no
 * spinlock.)
 * Sooo...
 * 	step 1. Make forks and nrees run longer with SAFEFORK_SPIN.
 * 	step 2. In thread A -> run bunch of reallocs.
 * 	step 3. In thread B -> run bunch of threads.
 * 	step 4. Hope both spinlocks will run.....
 * 	step 5. Stand up and turn around 2 times.
 * 	step 6. Sit down and check the results. If the program is still
 * 		running, then you have a spinlock. Patch it, then stand up
 * 		again and spin twice but now in the other direction.
 * */

long ffork_wait = 0;
long fnree_wait = 0;

#define SAFEFORK_HOOK
#define NREE_SPIN fnree_wait++
#define FORK_SPIN ffork_wait++
#define SAFEFORK_SPIN usleep(1)

#include "safefork.h"

pthread_t t[2];

void end_threads()
{
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);
}

void *t1(void *arg __attribute__((unused)))
{
	size_t i, len;
	void *ptr = NULL;
	len = 5;
	for (i = 0; i < len; i += 1) {
		ptr = realloc(ptr, i);
		usleep(1);
	}
	free(ptr);
	return NULL;
}

void *t2(void *arg __attribute__((unused)))
{
	size_t len;
	len = 5;
	while (len--) {fork(); usleep(1);}
	fork_cleanup();
	if (bad) {
		fprintf(stderr, "error@fork_cleanup\n");
		abort();
	}
	return NULL;
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);

	pthread_create(&t[0], NULL, &t1, NULL);
	pthread_create(&t[1], NULL, &t2, NULL);
	end_threads();

	/* If these values are more then 0, then at some point there was a 
	 * lock but it didn't end in a spinlock. */
	test(ffork_wait > 0);
	test(fnree_wait > 0);

	return EXIT_SUCCESS;
}
