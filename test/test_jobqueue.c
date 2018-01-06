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
	int i;
	thread_pool *pool = thpool_init(2);
	for (i = 0;i< 30;i++) {
		thpool_add_work(pool,run1,NULL);
	}
	thpool_wait(pool);
	return 0;
}