CC=gcc
CFLAGS = -g -O2 -I.
LDLIBS = -lpthread

PROGS = fork forkx2 forks myfork.so

all: csapp.o  $(PROGS)

myfork.so: myfork.c
	$(CC) $(CFLAGS) -shared -fpic -o myfork.so myfork.c -ldl

$(PROGS): csapp.o

clean:
	rm -rf $(PROGS) *~


