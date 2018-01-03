#include "threadpool.h"
int watch_flag_init(watch_flag * ptr_sign)
{
	ptr_sign = (struct watch_flag *)malloc(sizeof(struct watch_flag));
	if (ptr_sign == NULL) {
		return -1;
	}
	ptr_sign->flag = 0;
	pthread_mutex_init(&ptr_sign->sign_mutex,NULL);
	pthread_cond_init(&ptr_sign->sign_cond,NULL);
	return 0;
}