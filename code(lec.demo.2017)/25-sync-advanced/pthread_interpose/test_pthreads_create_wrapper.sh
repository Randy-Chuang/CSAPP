#!/bin/sh
echo "Testing the pthread_create wrapper implementation"
LD_PRELOAD="./pthreads-csapp-lib.so" ./test_pthread_create