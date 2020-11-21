CC = gcc

CFLAGS = -O1 -Wall -I .
LDLIBS = -lpthread -lm

COUNTERARGS = -O1 -Wall 

# NOTE: we expect sharing.c to generate warnings
PROGS =	badcnt\
	goodcnt\
	goodmcnt\
	sharing\

all: $(PROGS)

# The two programs that we use to illustrate synchronization
goodcnt: goodcnt.c csapp.c csapp.h
	$(CC) $(COUNTERARGS) -o goodcnt goodcnt.c csapp.c -lpthread
	$(CC) $(COUNTERARGS) -S goodcnt.c

goodmcnt: goodmcnt.c csapp.c csapp.h
	$(CC) $(COUNTERARGS) -o goodmcnt goodmcnt.c csapp.c -lpthread
	$(CC) $(COUNTERARGS) -S goodmcnt.c

badcnt: badcnt.c csapp.c csapp.h
	$(CC) $(COUNTERARGS) -o badcnt badcnt.c csapp.c -lpthread
	$(CC) $(COUNTERARGS) -S badcnt.c

sharing: sharing.c csapp.c csapp.h

clean:
	rm -f $(PROGS) *.o *.s *~
