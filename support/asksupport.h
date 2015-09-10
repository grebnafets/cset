#ifndef ASKSUPPORT
#define ASKSUPPORT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* info {{{1 */

#define INFO_FORK 1
/* info_fork {{{2 */
void info_fork()
{
	fprintf(stdout, "fork():\n");

	fprintf(stdout, "\t*USE WITH CARE\n");

	fprintf(stdout, "\t*fork copies entire process into a new process.\n");

	fprintf(stdout, "\t*fork returns pid_t twice where -1 means fail, ");
		fprintf(stdout, "0 means child and 0< means parent.\n");

	fprintf(stdout, "\t*fork copies on write, at least on linux.\n");

	fprintf(stdout, "\t*forket child will only have one main thread.\n");

	fprintf(stdout, "\t*If you really need to do something stupid with ");
		fprintf(stdout, "fork, then use the Dijkstra concept of ");
	      	fprintf(stdout, "semaphores. Otherwise, just call excec.\n");

	fprintf(stdout, "\t*fork might cause valgrind to output memory leak ");
		fprintf(stdout, "within child even if there is technically ");
		fprintf(stdout, "none. Example would be calling fork within ");
		fprintf(stdout, "pthread; that thread is going to ");
		fprintf(stdout, "become the main thread -> Who is going ");
		fprintf(stdout, "to clean up that thread?\n");
}
/* }}}2 */

/* }}}1 */

/* For developers.
 * Quick query to check support of function within platform or something...
 * For quick information about some function.
 * Not a good idea to use for production. */
void __ask(const char *question, const int answer)
{

	/* fork {{{ */

	if (!strcmp(question, "CAN_FORK")) {
		if (answer) {
			fprintf(stdout, "Yes, you can use fork\n");
		} else {
			fprintf(stdout, "No, you can not use fork\n");
		}
		goto RET;
	}

	if (!strcmp(question, "INFO_FORK"))  {
		info_fork();
		goto RET;
	}

	/* }}} */

RET:
	return;
}
#define ask(q) __ask(#q, q)


#endif /* ASKSUPPORT */
