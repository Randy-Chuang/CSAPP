#!/bin/sh
echo "Testing the pthread_cond_wait() wrapper implementation"
LD_PRELOAD="./pthreads.so" ./test_cond_wait