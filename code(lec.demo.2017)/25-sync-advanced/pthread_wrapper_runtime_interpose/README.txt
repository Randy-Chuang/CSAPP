This directory contains code to implement and test a library for
inserting millisecond-scale random delays into pthreads calls.  It
generates a library file suitable for use with runtime
interpositioning.

csapp.{c,h}:
	CSAPP wrapper library

pthreads-csapp-lib.c:
	code for wrapping pthread calls via runtime interposition.

test_cond_wait.c:
	Test with producer/consumer example using condition variables

test_pthread_create.c:
	Test with thread creation example

test_pthreads_cond_wait_wrapper.sh
test_pthreads_create_wrapper.sh
	Shell scripts demonstrating how to use interpositioning.
