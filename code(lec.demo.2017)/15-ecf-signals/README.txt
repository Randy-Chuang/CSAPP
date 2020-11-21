Programs used in lecture to demonstrate signals


csapp.{c,h}
        CS:APP3e code library

ms_delay.{c,h}
	Library to generate and insert random delays in code

myfork.c
	Library that can be used to interpose fork and pause using runtime inteposition
	After compiling, can enable with command:
	linux> setenv LD_PRELOAD ./myfork.so

	These environment variables can be set to control this library:
	VERBOSE   When set, print information about the operations
	CHILD     When set, disables delays along child side of fork
	PARENT	  When set, disables delays along parent side of fork
	PAUSE	  When set, disables inserted delays in pause().
	SLEEP     When set, uses nanosleep to delay execution.  Otherwise uses a spin loop.
		  Since sleeping terminates when a signal is received, spinning is a more
		  reliable choice.

execs.c
forks.c
        Demonstrate exec, fork, wait, and signals

shellex.c
        Simple shell example

sigint.c
sigintsafe.c
        Unsafe and safe versions of program that catches SIGINT

signaldeadlock.c

        Demonstrates deadlock caused by using unsafe printf in signal
        handler

procmask1.c
        Race condition caused by assuming parent runs before child
procmask2.c
        Correct version that eliminates the race        

waitforsignal.c
        Wastefull way to wait for signals 
sigsuspend.c
        Correct way to wait for signals using sigsuspend

setjmp.c
        Uses setjmp and longjmp to return from deeply nested function

restart.c
        Restarts when the user types ctrl-c

internal.c 
external.c 
           Programs using signals to respond to internal and external
           events
