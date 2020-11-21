CC=gcc
CFLAGS=-g -O2 -Wall
LDLIBS=-lpthread

all: echoclient echoserveri tinycode

tinycode:
	(cd tiny; make)

echoclient: echoclient.c csapp.h csapp.c
echoserveri: echoserveri.c echo.c csapp.h csapp.c

clean:
	rm -f *.o *~ *.exe echoclient echoserveri
	(cd tiny; make clean)

