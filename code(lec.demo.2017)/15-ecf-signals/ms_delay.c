/* lnterposition for fork */
#include "ms_delay.h"

/* Performance parameters */
/* Maximum delay (in milliseconds) */
#define MAX_DELAY 100

/* Global data */
static int max_delay = MAX_DELAY;
static bool initialized = false;

void set_max_delay(int d) {
    max_delay = d;
}

static void initialize() {
    if (initialized)
        return;
    srandom(getpid());
    initialized = true;
}

/* Add more entropy to RNG */
static void entropy() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    int n = t.tv_nsec % 25;
    while (n--)
	random();
}

/* Hard delay.  Won't get interrupted by a signal */
void ms_spin(int msecs) {
    struct timespec t;
    long start_ms;
    long curr_ms;
    clock_gettime(CLOCK_REALTIME, &t);
    start_ms = t.tv_sec * 1000 + t.tv_nsec/1000000;
    curr_ms = start_ms;
    while (curr_ms - start_ms < msecs) {
	clock_gettime(CLOCK_REALTIME, &t);
	curr_ms = t.tv_sec * 1000 + t.tv_nsec/1000000;
    }
}

/* Version of sleep that works at millisecond time scale */
void ms_sleep(int msecs, bool spin) {
    if (msecs == 0)
	return;
    if (spin) {
	ms_spin(msecs);
	return;
    }
	
    struct timespec req;
    long secs = msecs / 1000;
    long nsecs = (msecs - secs * 1000) * 1000000;
    req.tv_sec = secs;
    req.tv_nsec = nsecs;
    if (nanosleep(&req, NULL) < 0) {
        fprintf(stderr, "Couldn't sleep for %d milliseconds\n", msecs);
    }
}

/* Uniform random number in [0.0,1.0] */
static double uniform() {
    initialize();
    return (double) random() / RAND_MAX;
}

int choose_delay() {
    entropy();
    return (int) (uniform() * max_delay);
}

bool env_flag(char *name) {
    return getenv(name) != NULL;
}

bool choose_with_probability(double prob) {
    return uniform() <=  prob;
}
