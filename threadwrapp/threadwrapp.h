#ifndef THREADWRAPP
#define THREADWRAPP 1

#include "../assemblyline/assemblyline.h"
#include "../mem/mem.h"
#include <pthread.h>
#include "../safefork/safefork.h"

#ifndef MAIN_THREAD
pthread_t MAIN_THREAD = 0;
#endif /* MAIN_THREAD */

ASSEMBLY_UNIT *pthread_unit = NULL;
void **pthread_unit_arg = NULL;
size_t *pthreadwrapp_id = NULL;
size_t pthread_unit_len = 0;

struct assemblyline *pthread_pre  = NULL;
struct assemblyline *pthread_post = NULL;

void pthreadwrapp_init() __attribute__((constructor));
void pthreadwrapp_init()
{
	
	MAIN_THREAD  = pthread_self();
	pthread_pre  = new_assemblyline(NULL);
	pthread_post = new_assemblyline(NULL);
}

void pthreadwrapp_free() __attribute__((destructor));
void pthreadwrapp_free()
{
	free(pthread_unit);
	free(pthread_unit_arg);
	free(pthreadwrapp_id);
	pthread_unit_len = 0;
	free_assemblyline(pthread_pre);
	free_assemblyline(pthread_post);
}

#define pthread_push_pre(args...) __assemblyline_push(pthread_pre, ASSEMBLY_UNIT_NUMARGS(args), args)
#define pthread_push_post(args...) __assemblyline_push(pthread_post, ASSEMBLY_UNIT_NUMARGS(args), args)

void *pthreadwrapp(void *index)
{
	size_t i = (size_t)index;
	void *arg = pthread_unit_arg[i];
	arg = assemblyline_run_arg(pthread_pre, arg);
	arg = pthread_unit[i](arg);
	arg = assemblyline_run_arg(pthread_post, arg);
	return arg;
}

int pthreadwrapp_create(
	pthread_t *thread,
	const pthread_attr_t *attr,
	ASSEMBLY_UNIT unit,
	void *arg
)
{
	int ret;
	size_t index = pthread_unit_len++;
	size_t len   = index + 1;
	pthread_unit = (ASSEMBLY_UNIT *)mem.xr(
		pthread_unit, len * sizeof(ASSEMBLY_UNIT)
	);
	pthread_unit_arg = (void **)mem.xr(
		pthread_unit_arg, len * sizeof(void *)
	);
	pthreadwrapp_id  = (size_t *)mem.xr(
		pthreadwrapp_id, len * sizeof(size_t)
	);
	pthread_unit[index]     = unit;
	pthread_unit_arg[index] = arg;
	pthreadwrapp_id[index]  = index;
	ret = pthread_create(
		thread, attr, pthreadwrapp, (void *)pthreadwrapp_id[index]
	);
	return ret;
}

struct thr {
	int (*create)(
		pthread_t *thread,
		const pthread_attr_t *attr,
		void *(*start_routine)(void *), void *arg
	);
	int (*join)(pthread_t thread, void **retval);
} thr = {
	.create = &pthreadwrapp_create,
	.join   = &pthread_join
};

#endif /* THREADWRAPP */
