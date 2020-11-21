CC = gcc
CFLAGS=-g -O2 -Wall
LDLIBS = -lpthread

all: echoclient echoserveri echoserverp echoservert handler-deadlock race

echoclient: echoclient.c csapp.c csapp.h
echoserveri: echoserveri.c echo.c csapp.c csapp.h
echoserverp: echoserverp.c echo.c csapp.c csapp.h
echoservert: echoservert.c echo.c csapp.c csapp.h
handler-deadlock: handler-deadlock.c csapp.h
race: race.c csapp.c csapp.h

clean:
	rm -rf *~ echoclient echoserveri echoserverp echoservert handler-deadlock race *.o
