Examples of Linux process control from lecture

FILES

Makefile
        "make clean; make" to compile everything

csapp.{c,h} 
        CS:APP3e code library

fork.c 
        Simple example of fork

forkx2.c
	Another example of fork, demonstrating independence of state
forks.c
        Multiple examples of Linux process control
        Usage: ./forks <n>

myfork.c
	Custom version of fork that inserts random delays on parent and child branches.
	Use runtime interpositioning to make the above demonstration programs more interesting.
	Checks environment variables:
	       VERBOSE	   When set, prints out information about process IDs and inserted delays
	       CHILD	   When set, disables delay on child branch of fork
	       PARENT	   When set, disables delay on parent branch of fork

        

        
