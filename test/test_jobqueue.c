#include "threadpool.h"
#include <stdio.h>

void *run(void *arg) {
	printf("%s\n", "hello world!");
	return NULL;
}

int main(int argc, char const *argv[])
{
	job_queue *queue_ptr;
	job *job_ptr;
	int ret = job_queue_init(&queue_ptr);
	job_ptr = malloc(sizeof(job));
	job_ptr->func = run;
	job_ptr->arg = NULL;
	job_queue_push(queue_ptr, job_ptr);
	job_queue_push(queue_ptr, job_ptr);
	job_queue_push(queue_ptr, job_ptr);
	job_queue_push(queue_ptr, job_ptr);
	job_queue_push(queue_ptr, job_ptr);
	job *j = queue_ptr->front;
	j->func(j->arg);
	j = queue_ptr->front->next;
	j->func(j->arg);
	j = queue_ptr->front->next->next;
	j->func(j->arg);
	j = queue_ptr->front->next->next->next;
	j->func(j->arg);
	j = queue_ptr->front->next->next->next->next;
	j->func(j->arg);
	return 0;
}