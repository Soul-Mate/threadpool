#include <stdio.h>
#include <stdlib.h>
#include "threadpool.h"

static volatile int keep_alive = 0;

thread_pool * thpool_init(int nums)
{
	keep_alive = 1;
	int i;
	if (nums <= 0) {
		return NULL;
	}

	thread_pool * pool = (thread_pool*)malloc(sizeof(thread_pool));
	if (pool == NULL) {
		return NULL;
	}
	pool->work_num = 0;
	pool->alive_num = 0;
	pthread_mutex_init(&pool->pool_mutex, NULL);
	pthread_rwlock_init(&pool->pool_rwlock, NULL);
	pthread_cond_init(&pool->pool_cond, NULL);
	pool->threads = (thread**)malloc(sizeof(thread*) * nums);

	if (job_queue_init(&pool->thread_job_queue) != 0) {
		free(pool);
	}
	for (i = 0; i < nums; ++i) {
		thread_create(pool->threads[i], i, pool);
	}
	while (pool->alive_num != nums);
	return pool;
}

void thpool_wait(thread_pool *ptr_pool)
{
	pthread_cond_wait(&ptr_pool->pool_cond,&ptr_pool->pool_mutex);
}

int thpool_add_work(thread_pool *pool, void *(*func)(void *arg), void *arg)
{
	job *ptr_job = (job *)malloc(sizeof(job));
	if (ptr_job == NULL) {
		return -1;
	}
	ptr_job->func = func;
	ptr_job->arg = arg;
	job_queue_push(pool->thread_job_queue, ptr_job);
}

int thread_create(thread *ptr_thread, int i, thread_pool *pool)
{

	ptr_thread = (thread *)malloc(sizeof(thread));
	ptr_thread->id = i;
	pthread_create(&(ptr_thread->tid), NULL, thread_run, (void *)pool);
	return 0;
}

void *thread_run(void *arg)
{
	thread_pool *pool = (thread_pool *)arg;
	pthread_rwlock_wrlock(&pool->pool_rwlock);
	pool->alive_num++;
	pthread_rwlock_unlock(&pool->pool_rwlock);

	for (;;) {
		job_flag_wait(pool->thread_job_queue->has_job);

		if (keep_alive) {
			pthread_rwlock_wrlock(&pool->pool_rwlock);
			pool->work_num++;
			pthread_rwlock_unlock(&pool->pool_rwlock);

			job *ptr_job = job_queue_pull(pool->thread_job_queue);
			if (ptr_job) {
				ptr_job->func(ptr_job->arg);
				free(ptr_job);
			}
			
			pthread_rwlock_wrlock(&pool->pool_rwlock);
			pool->work_num--;
			if (!pool->work_num) {
				pthread_cond_signal(&pool->pool_cond);
			}
			pthread_rwlock_unlock(&pool->pool_rwlock);
		}
	}
	pthread_rwlock_wrlock(&pool->pool_rwlock);
	pool->alive_num--;
	pthread_rwlock_unlock(&pool->pool_rwlock);
}


