#define SAFEFORK_HOOK
#include <cset/test/test.h>
#include <cset/threadwrapp/threadwrapp.h>
#include <sys/types.h>
#include <sys/wait.h>

void test_dep()
{
	test(DEP FOUND ASSEMBLYLINE);
	test(DEP FOUND MEM);
}

struct targ {
	int a, b, c, d, e;
};

void *pre1(void *arg)
{
	#define PRE1 1
	test(PRE1);
	#undef PRE1
	struct targ *q = (struct targ *)arg;
	q->a = 1;
	return (void *)q;
}

void *pre2(void *arg)
{
	#define PRE2 1
	test(PRE2);
	#undef PRE2
	struct targ *q = (struct targ *)arg;
	q->b = 1;
	return (void *)q;
}

void *thread(void *arg)
{
	#define THE_THREAD 1
	test(THE_THREAD);
	#undef THE_THREAD
	struct targ *q = (struct targ *)arg;
	q->c = 1;
	return (void *)q;
}

void *post1(void *arg)
{
	#define POST1 1
	test(POST1);
	#undef POST1
	struct targ *q = (struct targ *)arg;
	q->d = 1;
	return (void *)q;
}

void *post2(void *arg)
{
	#define POST2 1
	test(POST2);
	#undef POST2
	struct targ *q = (struct targ *)arg;
	q->e = 1;
	return (void *)q;
}

void test_thread_preandpost()
{
	pthread_push_pre(&pre1, &pre2);
	pthread_push_post(&post1, &post2);
	pthread_t t[3];
	struct targ *arg1 = mem.m(sizeof(struct targ));
	struct targ *arg2 = mem.m(sizeof(struct targ));
	struct targ *arg3 = mem.m(sizeof(struct targ));
	arg1->a = arg1->b = arg1->c = arg1->d = arg1->e = 0;
	arg2->a = arg2->b = arg2->c = arg2->d = arg2->e = 0;
	arg3->a = arg3->b = arg3->c = arg3->d = arg3->e = 0;
	thr.create(&t[0], NULL, &thread, arg1);
	thr.create(&t[1], NULL, &thread, arg2);
	thr.create(&t[2], NULL, &thread, arg3);
	thr.join(t[0], NULL);
	thr.join(t[1], NULL);
	thr.join(t[2], NULL);
	test(arg1->a);test(arg1->b);test(arg1->c);test(arg1->d);test(arg1->e);
	test(arg2->a);test(arg2->b);test(arg2->c);test(arg2->d);test(arg2->e);
	test(arg3->a);test(arg3->b);test(arg3->c);test(arg3->d);test(arg3->e);
	free(arg1);free(arg2);free(arg3);
}

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "threadwrapp");
	test_dep();
	#ifdef CAN_FORK
	pid_t id = fork();
	if (id == 0) {
		test_thread_preandpost();
	} else {
		test_thread_preandpost();
		wait(&id);
	}
	#else
	test_thread_preandpost();
	#endif /* CAN_FORK */
	return EXIT_SUCCESS;
}
