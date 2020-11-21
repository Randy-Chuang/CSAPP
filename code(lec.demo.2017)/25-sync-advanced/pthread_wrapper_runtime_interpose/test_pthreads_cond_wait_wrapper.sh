#!/bin/sh
echo "Testing the pthread_cond_wait() wrapper implementation"
LD_PRELOAD="./pthreads-csapp-lib.so" ./test_cond_wait