CC = gcc
CFLAGS = -Og -g
LDLIBS = -lpthread -lrt

PROGS = forks execs internal shellex signaldeadlock loop sigint sigintsafe procmask1 procmask2 waitforsignal sigsuspend setjmp restart delay myfork.so

all: $(PROGS)

myfork.so: myfork.c ms_delay.h ms_delay.c

	$(CC) $(CFLAGS) -shared -fpic -o myfork.so myfork.c ms_delay.c -ldl -lrt

process-race: process-race.c ms_delay.c csapp.c


signaldeadlock: signaldeadlock.c csapp.c
shellex: shellex.c csapp.c
forks: forks.c csapp.c
execs: execs.c
internal: internal.c csapp.c
sigint: sigint.c csapp.c
sigintsafe: sigintsafe.c csapp.c
loop: loop.c
procmask1: procmask1.c csapp.c
procmask2: procmask2.c csapp.c
waitforsignal: waitforsignal.c csapp.c
sigsuspend: sigsuspend.c csapp.c
setjmp: setjmp.c csapp.c
restart: restart.c csapp.c

clean:
	rm -rf *~ $(PROGS)

