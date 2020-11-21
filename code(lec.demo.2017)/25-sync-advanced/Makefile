CC = gcc

CFLAGS = -O0 -Wall -g
LDLIBS = -lpthread -lrt

PROGS =	race \
	norace\
	produce-consume \
	read-write \
	deadlock \


all: $(PROGS)

# Dependencies
race: race.c csapp.h csapp.c
norace: norace.c csapp.h csapp.c
produce-consume: produce-consume.c csapp.h csapp.c sbuf.h sbuf.c ms_delay.h ms_delay.c
read-write: read-write.c csapp.h csapp.c ms_delay.h ms_delay.c
deadlock: deadlock.c csapp.h csapp.c ms_delay.h ms_delay.c

clean:
	rm -f $(PROGS) *.o *~
