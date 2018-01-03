#include "threadpool.h"
#include <stdio.h>

void *run1(void *arg) {
	printf("%s\n", "call run1");
	return NULL;
}

void *run2(void *arg) {
	printf("%s\n", "call run2");
	return NULL;
}


void *run3(void *arg) {
	printf("%s\n", "call run3");
	return NULL;
}

int main(int argc, char const *argv[])
{
	job_queue *queue_ptr;
	job *j1,*j2,*j3;
	job_queue_init(&queue_ptr);
	j1 = malloc(sizeof(job));
	j1->func = run1;
	j1->arg = NULL;
	j2 = malloc(sizeof(job));
	j2->func = run2;
	j2->arg = NULL;
	j3 = malloc(sizeof(job));
	j3->func = run3;
	j3->arg = NULL;
	job_queue_push(queue_ptr,j1);
	job_queue_push(queue_ptr,j2);
	job_queue_push(queue_ptr,j3);
	while(queue_ptr->len) {
		job *job_p = job_queue_pull(queue_ptr);
		if (job_p) {
			job_p->func(job_p->arg);
			free(job_p);
		}
	}
	return 0;
}