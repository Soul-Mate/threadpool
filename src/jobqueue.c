#include "threadpool.h"
#include <stdio.h>
int job_flag_init(job_flag * ptr_flag)
{
	ptr_flag = (job_flag *)malloc(sizeof(job_flag));
	if (ptr_flag == NULL) {
		return -1;
	}
	ptr_flag->flag = 0;
	pthread_mutex_init(&ptr_flag->sign_mutex,NULL);
	pthread_cond_init(&ptr_flag->sign_cond,NULL);
	return 0;
}


int job_queue_init(job_queue **ptr_queue)
{
	(*ptr_queue) = (job_queue *)malloc(sizeof(job_queue));
	if ((*ptr_queue) == NULL){
		return -1;
	}
	job_flag * ptr_flag;
	(*ptr_queue)->len = 0;
	(*ptr_queue)->front = NULL;
	(*ptr_queue)->rear = NULL;
	if (job_flag_init(ptr_flag) < 0) {
		return -1;
	}
	(*ptr_queue)->has_job = ptr_flag;
	pthread_rwlock_init(&(*ptr_queue)->queue_rwlock,NULL);
	return 0;
}

void job_queue_push(job_queue *ptr_queue, job *ptr_job)
{
	switch(ptr_queue->len) {
		case 0:
			ptr_queue->front = ptr_job;
			ptr_queue->rear = ptr_job;
			break;
		default:
			ptr_queue->rear->next = ptr_job;
            ptr_queue->rear = ptr_job;
	}
	ptr_queue->len++;
}


job *job_queue_pull(job_queue *ptr_queue)
{
	job *out_ptr_job;
	switch(ptr_queue->len) {
		case 0:
			out_ptr_job = NULL;
			break;
		case 1:
			out_ptr_job = ptr_queue->front;
			ptr_queue->front = NULL;
			ptr_queue->rear = NULL;
			ptr_queue->len = 0;
			break;
		default:
			out_ptr_job = ptr_queue->front;
			ptr_queue->front = out_ptr_job->next;
			ptr_queue->len--;
	}
	return out_ptr_job;
}