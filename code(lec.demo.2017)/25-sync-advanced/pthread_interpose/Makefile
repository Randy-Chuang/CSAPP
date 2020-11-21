CC = /usr/bin/gcc
CFLAGS = -g -Wall -Werror -std=gnu99 -O2
LIBS = -lpthread
#DEFINES = -DDEBUG

FILES = pthreads.so test_cond_wait test_cond_wait_link test_pthread_create

all: $(FILES)
	cp -p pthreads.so ..

#
# Using link-time interpositioning to introduce non-determinism in the
# order that parent and child execute after invoking pthread_lib
#
test_pthread_create: test_pthread_create.c 
	$(CC) $(CFLAGS) -o test_pthread_create test_pthread_create.c csapp.c $(LIBS)

test_cond_wait_link: test_cond_wait.c pthread_cond_wait_link_interpose.c
	$(CC) $(CFLAGS) -Wl,--wrap,pthread_cond_wait -o test_cond_wait_link test_cond_wait.c pthread_cond_wait_link_interpose.c $(LIBS)	

test_cond_wait: test_cond_wait.c
	$(CC) $(CFLAGS) -o test_cond_wait test_cond_wait.c $(LIBS)	

pthreads.so: pthreads-csapp-lib.c
	$(CC) $(DEFINES) -shared -fpic -o pthreads.so pthreads-csapp-lib.c -ldl -lpthread


# Clean up
clean:
	rm -f $(FILES) *.o *~ *.so