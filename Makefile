test_jobqueue: watchflag.o jobqueue.o
	gcc obj/*.o -Iinc -lpthread test/test_jobqueue.c -o bin/test_jobqueue

jobqueue.o: src/jobqueue.c
	gcc -c src/jobqueue.c -Iinc -lpthread -o obj/jobqueue.o

watchflag.o: src/watchflag.c
	gcc -c src/watchflag.c -Iinc -lpthread -o obj/watchflag.o