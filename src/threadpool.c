#include <stdio.h>
#include <stdlib.h>
#include "threadpool.h"

static int keep_alive = 0;

int thpool_init(int nums)
{
	int i;
	if (nums <= 0) {
		return -1;
	}
	/* init thread pool */
	th_pool * pool = (th_pool*)malloc(sizeof(th_pool));
	if (pool == NULL) {
		return -1;
	}
	pool->work_num = 0;
	pool->alive_num = 0;
	pthread_mutex_init(&pool->pool_mutex,NULL);
	pthread_rwlock_init(&pool->pool_rwlock,NULL);
	pthread_cond_init(&pool->pool_cond,NULL);
	pool->threads = (th**)malloc(sizeof(th*) * nums);
	
	/*create thread*/
	for (i = 0; i < nums; ++i) {
		thread_create(pool->threads[i],i,pool);
	}
	while(1); // TODO test thread run
	return 0;
}

int thread_create(th *thread, int i, th_pool *pool)
{
	thread = (th *)malloc(sizeof(th));
	thread->id = i;
	pthread_create(&(thread->tid),NULL,thread_run,(void *)pool);
	return 0;
}

void *thread_run(void *arg)
{
	th_pool *pool = (th_pool *)arg;
	pthread_mutex_lock(&pool->pool_mutex);
	printf("%s\n", "hello world");
	pthread_mutex_unlock(&pool->pool_mutex);
}
