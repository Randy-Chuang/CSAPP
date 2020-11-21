CC = gcc

CFLAGS = -O1 -Wall


psum: psum.c csapp.o csapp.h realtimer.o realtimer.h
	$(CC) $(CFLAGS) -o psum psum.c csapp.o realtimer.o -lpthread

psum-data.txt: psum psum-run.pl
	./psum-run.pl > psum-data.txt

psum-spacing.txt: psum psum-spacing.pl
	./psum-spacing.pl > psum-spacing.txt

psum.s: psum.c
	$(CC) $(CFLAGS) -S psum.c

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

realtimer.o: realtimer.c realtimer.h
	$(CC) $(CFLAGS) -c realtimer.c

pqsort.o: pqsort.c pqsort.h
	$(CC) $(CFLAGS) -c pqsort.c

sortbench: sortbench.c csapp.o realtimer.o pqsort.o
	$(CC) $(CFLAGS) -o sortbench sortbench.c csapp.o realtimer.o pqsort.o -lpthread

sortbench-data.txt: sortbench sortbench-run.pl
	./sortbench-run.pl > sortbench-data.txt

clean:
	rm -rf psum sortbench *.o *~

