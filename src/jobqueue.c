#include "threadpool.h"

int job_queue_init(job_queue **queue)
{
	(*queue) = (job_queue *)malloc(sizeof(struct job_queue));
	if ((*queue) == NULL) {
		return -1;
	}
	(*queue)->len = 0;
	(*queue)->front = NULL;
	(*queue)->rear = NULL;
	watch_flag * ptr_flag;
	if (watch_flag_init(ptr_flag) < 0) {
		return -1;
	}
	(*queue)->has_job = ptr_flag;
	pthread_rwlock_init(&(*queue)->queue_rwlock, NULL);
	return 0;
}

void job_queue_push(job_queue *ptr_queue, job *ptr_job)
{
	switch (ptr_queue->len) {
	case 0:
		ptr_queue->front = ptr_job;
		ptr_queue->rear = ptr_job;
		break;
	default:
		ptr_job->next = ptr_queue->front;
		ptr_queue->front = ptr_job;
	}
	ptr_queue->len++;
}
