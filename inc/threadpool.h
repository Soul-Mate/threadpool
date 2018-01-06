

#ifndef _THREAD_POOL
#define _THREAD_POOL

#ifndef DEBUG
#define DEBUG
#endif /*DEBUG*/

#include <pthread.h>
#include <stdlib.h>

/*job sign struct*/
typedef struct _job_flag {
	int flag;
	pthread_mutex_t sign_mutex;
    pthread_cond_t sign_cond;
}job_flag;

/* job node struct */
typedef struct _job{
	void *arg;
	void *(*func)(void *arg);
	struct _job *next;
}job;

/* job queue struct */
typedef struct job_queue {
	int len;
	job *front;
	job *rear;
	job_flag *has_job;
	pthread_rwlock_t queue_rwlock;
}job_queue;

/*thread struct*/
typedef struct _thread {
	int id;			/* thread sequence number */
	pthread_t tid;	/* thread thread_t */
}thread;

/* thread pool struct */
typedef struct _thread_pool {
	volatile int work_num;
	volatile int alive_num;
	pthread_rwlock_t pool_rwlock;
	pthread_mutex_t pool_mutex;
	pthread_cond_t pool_cond;
	thread **threads;
	job_queue *thread_job_queue;
}thread_pool;

int job_flag_init(job_flag **);
void job_flag_wait(job_flag *);
void job_flag_tell(job_flag *);


thread_pool * thpool_init(int);
void thpool_wait(thread_pool *);
int thpool_add_work(thread_pool *pool,void *(*func)(void *),void *);
void thpool_resume(thread_pool *pool);
void thpool_pasue(thread_pool *pool);


int thread_create(thread *, int, thread_pool *);
void *thread_run(void *);


int job_queue_init(job_queue **);
void job_queue_push(job_queue *, job *);
job * job_queue_pull(job_queue *);

#endif /*_THREAD_POOL*/