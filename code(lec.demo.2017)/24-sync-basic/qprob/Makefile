CC = gcc

CFLAGS = -g -O0 -Wall -I .
LDLIBS = -lpthread -lm

COUNTERARGS = -O1 -Wall 

# NOTE: we expect sharing.c to generate warnings
PROGS =	sharing

all: $(PROGS)

sharing: sharing.c csapp.c csapp.h
	$(CC) $(CFLAGS) -o sharing sharing.c csapp.c $(LDLIBS)

clean:
	rm -f $(PROGS) *.o *.s *~
