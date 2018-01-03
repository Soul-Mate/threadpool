

#ifndef _THREAD_POOL
#define _THREAD_POOL

#ifndef DEBUG
#define DEBUG
#endif /*DEBUG*/

#include <pthread.h>
#include <stdlib.h>

/*job sign struct*/
typedef struct watch_flag {
	int flag;
	pthread_mutex_t sign_mutex;
    pthread_cond_t sign_cond;
}watch_flag;

/* job node struct */
typedef struct job{
	void *arg;
	void *(*func)(void *arg);
	struct job *next;
}job;

/* job queue struct */
typedef struct job_queue {
	int len;
	job *front;
	job *rear;
	watch_flag *has_job;
	pthread_rwlock_t queue_rwlock;
}job_queue;

/*thread struct*/
typedef struct th {
	int id;			/* thread sequence number */
	pthread_t tid;	/* thread thread_t */
}th;

/* thread pool struct */
typedef struct th_pool {
	volatile int work_num;
	volatile int alive_num;
	pthread_rwlock_t pool_rwlock;
	pthread_mutex_t pool_mutex;
	pthread_cond_t pool_cond;
	th **threads;
	job_queue *thread_job_queue;
}th_pool;

int thpool_init(int nums);
int thread_create(th *thread, int i, th_pool *pool);
void *thread_run(void *arg);
int watch_flag_init(watch_flag *);
int job_queue_init(job_queue **);
void job_queue_push(job_queue *ptr_queue, job *ptr_job);
#endif /*_THREAD_POOL*/