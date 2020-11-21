/* lnterposition for fork */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>

/*
 * This code implements a custom version of fork() that inserts random delays
 * along the two sides of a fork.
 *
 * It checks environment variables:
 *             VERBOSE     When set, prints out information about process IDs and inserted delays
 *             CHILD       When set, disables delay on child branch of fork
 *             PARENT      When set, disables delay on parent branch of fork
 */            



/* Performance parameters */
/* Maximum delay (in milliseconds) inserted into each branch of fork */
#define MAX_DELAY 100

/* Global data */
bool initialized = false;
bool verbose = false;
bool delay_parent = true;
bool delay_child = true;

typedef pid_t (*fork_fun_t)(void);

static fork_fun_t real_fork;

/* Forward declaration */
/* Sleep for specified number of milliseconds */
static void ms_sleep(int msecs);

/* Choose a random delay */
static int choose_delay();

static void initialize() {
    if (initialized)
        return;
    char *error;
    real_fork = dlsym(RTLD_NEXT, "fork");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Couldn't link fork: %s\n", error);
        exit(1);
    }
    verbose = getenv("VERBOSE") != NULL;
    delay_parent = getenv("PARENT") == NULL;
    delay_child = getenv("CHILD") == NULL;
    srandom(getpid());
    initialized = true;
}

/* fork wrapper function */
pid_t fork(void) {
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
        ms_sleep(parent_delay);
    } else {
        /* Child */
        ms_sleep(child_delay);
    }
    return child_pid_or_zero;
}

/* Version of sleep that works at millisecond time scale */
static void ms_sleep(int msecs) {
    struct timespec req;
    long secs = msecs / 1000;
    long nsecs = (msecs - secs * 1000) * 1000000;
    req.tv_sec = secs;
    req.tv_nsec = nsecs;
    if (nanosleep(&req, NULL) < 0 && verbose) {
        fprintf(stderr, "Couldn't sleep for %d milliseconds\n", msecs);
    }
}

static int choose_delay() {
    pid_t pid = getpid();
    int i;
    /* Cycle through random numbers to make more nondeterministic */
    for (i = 0; i < pid % 30; i++)
        random();
    return random() % MAX_DELAY;
}
