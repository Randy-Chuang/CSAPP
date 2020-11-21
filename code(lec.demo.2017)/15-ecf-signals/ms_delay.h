/* Provide a random millisecond-level delay */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

/* Setting parameters */

/* Set maximum timer delay.  Default = 100 ms */
void set_max_delay(int ms);

/* Choose a random value between 0 and max milliseconds */
int choose_delay();

/* Sleep (or spin) for specified number of milliseconds */
void ms_sleep(int ms, bool spin);

/* Determine if environment value set */
bool env_flag(char *name);

/* Make a boolean choice */
bool choose_with_probability(double prob);

