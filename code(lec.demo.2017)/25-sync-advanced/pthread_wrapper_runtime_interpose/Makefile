CC = /usr/bin/gcc
CFLAGS = -g -Wall -Werror -std=gnu99
LIBS = -lpthread

FILES = test_pthread_create test_cond_wait pthreads-csapp-lib.so

all: $(FILES)

#
# Using link-time interpositioning to introduce non-determinism in the
# order that parent and child execute after invoking pthread_lib
#
test_pthread_create: test_pthread_create.c 
	$(CC) $(CFLAGS) -o test_pthread_create test_pthread_create.c csapp.c $(LIBS)

test_cond_wait: test_cond_wait.c
	$(CC) $(CFLAGS) -o test_cond_wait test_cond_wait.c csapp.c $(LIBS)	

pthreads-csapp-lib.so: pthreads-csapp-lib.c
	$(CC) -DRUNTIME -shared -fpic -o pthreads-csapp-lib.so pthreads-csapp-lib.c -ldl -lpthread

# Clean up
clean:
	rm -f $(FILES) *.o *~ *.so