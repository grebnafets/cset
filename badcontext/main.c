#include "../test/test.h"
#include "badcontext.h"

/* Demo use. {{{  */

/* Context definition, what we really care about. */
const int SHIT_HAPPENS  = __COUNTER__;
const int STATE_CHANGED = __COUNTER__;

/* Set context value. */
void setSHIT_HAPPENS()
{
	/* Check for something here? */
	cntxt = SHIT_HAPPENS;
	bad   = 1;
	/* Callback here maybe? */
}
void setSTATE_CHANGED()
{
	/* It doesn't have to be bad... */
	cntxt = STATE_CHANGED;
}

/* Set message or callback (This is up to you.) */

/* }}} */

/* tests {{{ */
void test_badcontext()
{
#define EQUALS ==
/* just kidding <|;o) */
#undef EQUALS

	test(SHIT_HAPPENS > 0);
	test(STATE_CHANGED > 0);

	/* Before SHIT_HAPPENS
	 * Should also be true within threads or forks. */
	test(bad == NOTBAD);
	test(cntxt == 0);
	setSHIT_HAPPENS();
	/* After SHIT_HAPPENS */
	test(cntxt == SHIT_HAPPENS);
	test(is(SHIT_HAPPENS));
	test(cntxt > 0);
	test(bad == BAD);
	setSTATE_CHANGED();
	/* After STATE_CHANGED */
	test(cntxt == STATE_CHANGED);
	test(is(STATE_CHANGED));
	test(cntxt > 0);
}
/* }}} */

/* test cases for thread and fork {{{1 */

/* thread setup {{{2 */
void *thread(void *arg __attribute__((unused)))
{
	test_badcontext();
	return NULL;
}
/* }}}2 */

void test_thread()
{
	pthread_t t;
	test_badcontext();
	pthread_create(&t, NULL, &thread, NULL);
	pthread_join(t, NULL);
}

void test_with_fork_and_threads()
{
#define CHILD 0
	pid_t id = fork();
	if (id == CHILD) {
		fprintf(stdout, "Child process\n");
		test_thread();
	} else {
		fprintf(stdout, "Parent process\n");
		test_thread();
	}
	int status;
	waitpid(id, &status, 0);
#undef CHILD
}

/* }}}1 */

void reset()
{
	cntxt = 0;
	bad   = 0;
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode);

	test_badcontext();
	reset();
#ifdef CAN_FORK
	test_with_fork_and_threads();
#else
	test_thread();
#endif /* CAN_FORK */

	return EXIT_SUCCESS;
}
