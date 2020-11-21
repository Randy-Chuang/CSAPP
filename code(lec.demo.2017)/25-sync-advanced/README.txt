Synchronization examples

csapp.{c,h}    
        CS:APP3e code library

ms_delay.{c,h}
	Code for inserting millisecond-scale delays into program, including randomly selected ones

sbuf.{c,h}
	Circular buffer code suitable for producer/consumer programs

race.c	
        Testing code for race conditions in pthread_create

norace.c
	Eliminate race condition by passing thread arguments using malloc'ed int's

produce-consume.c:
	Demonstration of producer/consumer code using sbuf

rw1.c:
	Readers/writers code from CS:APP3e

read-write.c:
	Demonstration of readers/writers code

deadlock.c:
	Demonstration of deadlock possibilities when different threads
	acquire locks in different orders
