CC = gcc
CFLAGS = -g 

FILES = sq struct

all: $(FILES)

sq: sq.c
	$(CC) $(CFLAGS) -o sq sq.c

struct: struct.c
	$(CC) $(CFLAGS) -o struct struct.c

clean:
	rm -f *~ *.o sq struct
	rm -rf *.dSYM

