/* lnterposition for fork */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>

#include "ms_delay.h"

/*
 * This code implements a custom version of fork() that inserts random delays
 * along the two sides of a fork.
 *
 * It also implements a custom version of pause() that inserts a random delay first
 *
 * It checks environment variables:
 *             VERBOSE     When set, prints out information about process IDs and inserted delays
 *             CHILD       When set, disables delay on child branch of fork
 *             PARENT      When set, disables delay on parent branch of fork
 */            




/* Global data */
static bool initialized = false;
static bool verbose = false;
static bool delay_parent = true;
static bool delay_child = true;
static bool delay_pause = true;
/* Spin, rather than nanosleep */
static bool do_sleep = false;

typedef pid_t (*fork_fun_t)(void);
typedef int (*pause_fun_t)(void);

static fork_fun_t real_fork;
static pause_fun_t real_pause;

static void initialize() {
    if (initialized)
        return;
    char *error;
    real_fork = dlsym(RTLD_NEXT, "fork");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Couldn't link fork: %s\n", error);
        exit(1);
    }
    real_pause = dlsym(RTLD_NEXT, "fork");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Couldn't link pause: %s\n", error);
        exit(1);
    }
    verbose = env_flag("VERBOSE");
    do_sleep = env_flag("SLEEP");
    delay_parent = !env_flag("PARENT");
    delay_child = !env_flag("CHILD");
    delay_pause = !env_flag("PAUSE");

    initialized = true;
}

/* fork wrapper function */
pid_t fork(void)
{
    initialize();
    int parent_delay = delay_parent ? choose_delay() : 0;
    int child_delay = delay_child ? choose_delay() : 0;
    pid_t parent_pid = getpid();
    pid_t child_pid_or_zero = real_fork();
    if (child_pid_or_zero > 0) {
        /* Parent */
        if (verbose) {
            printf(
   "Fork.  Child pid=%d, delay = %dms.  Parent pid=%d, delay = %dms\n",
                   child_pid_or_zero, child_delay, parent_pid, parent_delay);
            fflush(stdout);
        }
        ms_sleep(parent_delay, !do_sleep);
    } else {
        /* Child */
        ms_sleep(child_delay, !do_sleep);
    }
    return child_pid_or_zero;
}

/* pause wrapper function */
int pause(void) 
{
    initialize();
    int pause_delay = delay_pause ? choose_delay() : 0;
    pid_t pid = getpid();
    if (verbose) {
	printf("Pause, pid=%d, pre-delay=%dms.\n", pid, pause_delay);
    }
    ms_sleep(pause_delay, !do_sleep);
    return real_pause();
}
