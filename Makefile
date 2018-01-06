test_jobqueue: threadpool.o jobqueue.o
	gcc obj/*.o -Iinc -lpthread -g test/test_jobqueue.c -o  bin/test_jobqueue

threadpool.o: src/threadpool.c
	gcc -c src/threadpool.c -Iinc -lpthread -o obj/threadpool.o

jobqueue.o: src/jobqueue.c
	gcc -c src/jobqueue.c -Iinc -lpthread -o obj/jobqueue.o