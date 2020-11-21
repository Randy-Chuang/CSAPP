/* Implementation of simple parallel quicksort */

#include "csapp.h"
#include "taskq.h"
#include "pqsort.h"

/* Tuning parameters */
/* Verbosity level */
int verbose = 0;
/* Use library quicksort or homebrewed one? */
int use_qsort_lib = 0;
/* How small do partitions need to be relative to original size
   before using sequential sort & sequential partititoning */
size_t serial_fraction = 32;
int using_threads = 0;

/* Task queue for overall sort operation */
task_queue_ptr tq = NULL;

/* Debugging support */
/* For tracking debugging information */
static data_t *global_base = NULL;
static data_t *global_end = NULL;

size_t global_index(data_t *p) {
  return p - global_base;
}

#if 0
#define CHK(p) check_range(p)
#else
#define CHK(p) p
#endif

/* Break on this function */
static void breaker() {
  exit(0);
}

/* Make sure pointer is within range of data */
static data_t *check_range(data_t *p) {
  if (p < global_base || p > global_end) {
    printf("Error: Attempt to access element %ld\n",
	   (long int) global_index(p));
    breaker();
  }
  return p;
}

/*
  Partition size when start doing sequential sort
  (Computed at beginning)
*/
static size_t nele_max_serial = (1UL << 24);

/* Comparision function for qsort */
int dcomp(const void *p1, const void *p2) {
  data_t x1 = *CHK((data_t *) p1);
  data_t x2 = *CHK((data_t *) p2);
  if (x1 < x2)
    return -1;
  if (x1 == x2)
    return 0;
  return 1;
}

void qsort_lib(data_t *base, size_t nele) {
  qsort((void *) base, nele, sizeof(data_t), dcomp);
}

/* Pick pivot for quicksort */
static data_t *pivot(data_t *base, size_t nele) {
  return base + nele/2;
}

static void swap(data_t *p1, data_t *p2) {
  data_t x1 = *CHK(p1);  data_t x2 = *CHK(p2);
  *p1 = x2; *p2 = x1;
}

/* Partition array into elements smaller, equal, and greater than pivot.
   Return index of pivot element 
*/
static size_t partition_serial(data_t *base, size_t nele);
static size_t partition_parallel(data_t *base, size_t nele);

static size_t partition(data_t *base, size_t nele) {
  return partition_serial(base, nele);
}

static size_t partition_serial(data_t *base, size_t nele) {
  /* Select pivot */
  data_t *p = pivot(base, nele);
  /* Move to right of array */
  data_t pval = *CHK(p);
  swap(p, base+nele-1);
  data_t *left = base;
  data_t *right = base+nele-2;
  while (1) {
    while (left <= right && *CHK(left) <= pval)
      left++;
    while (left <= right && *CHK(right) >= pval)
      right--;
    if (left < right) {
      swap(left, right);
      left++;
      right--;
    } else
      break;
  }
  swap(left, base+nele-1);
  size_t pindex = left - base;
  if (verbose >= 3) {
    size_t l = global_index(base);
    size_t m = global_index(base+pindex);
    size_t r = global_index(base + nele - 1);
    printf("Serial partition.  [%lu,%lu,%lu]\n",
	   (printi_t) l, (printi_t) m, (printi_t) r);
  }
  return pindex;
}

/* Parallel partitioning
   Choose global pivot p.
   Break into k blocks and proceed in two phases:
   Phase 1: serially partition each block by p.  Collect pivot positions.
   Determine from pivots how to position subblocks.
   Phase 2: Copy subblocks into correct positions
*/

void qsort_serial(data_t *base, size_t nele) {
  if (verbose >= 3) {
    size_t l = global_index(base);
    size_t r = global_index(base + nele - 1);
    printf("Qsort_serial.  [%lu,%lu]\n", (printi_t) l, (printi_t) r);
  }
  if (nele <= 1)
    return;
  if (nele == 2) {
    if (base[0] > base[1])
      swap(base, base+1);
    return;
  }
  size_t m = partition_serial(base, nele);
  if (m > 1)
    qsort_serial(base, m);
  if (nele-1 > m+1)
    qsort_serial(base+m+1, nele-m-1);
}

/* For threaded sorting, have structure that defines sorting task */
typedef struct {
  data_t *base;
  size_t nele;
} sort_task_t, *sort_task_ptr;

static void init_task(size_t nele) {
  if (serial_fraction > 1) {
    nele_max_serial = nele / serial_fraction;
    if (nele_max_serial < 2)
      nele_max_serial = 2;
  }
  else
    nele_max_serial = nele;
  if (verbose >= 1) {
    printf("Serial cutover = %lu\n", (printi_t) nele_max_serial);
  }
  if (nele_max_serial < nele) {
    tq = new_task_queue();
    using_threads = 1;
  }
}

/* Add a new task to the list.  Returns position in task list */
static sort_task_ptr new_task(data_t *base, size_t nele) {
  sort_task_ptr t = Malloc(sizeof(sort_task_t));
  t->base = base;
  t->nele = nele;
  if (verbose >= 3) {
    size_t l = global_index(base);
    size_t r = global_index(base + nele-1);
    printf("New_task. [%lu,%lu]\n", (printi_t) l, (printi_t) r);
  }
  return t;
}

static void tqsort_helper(data_t *base, size_t nele);

/* Thread routine for many-threaded quicksort */
static void *sort_thread(void *vargp) {
  sort_task_t *t = (sort_task_t *) vargp;
  data_t *base = t->base;
  size_t nele = t->nele;
  free(vargp);
  if (verbose >= 2) {
    size_t l = global_index(base);
    size_t r = global_index(base + nele-1);
    printf("sort_thread.  tid=0x%lx  [%lu,%lu]\n",
	   (printi_t) pthread_self(), (printi_t) l, (printi_t) r);
  }
  size_t m = partition(base, nele);
  if (m > 1)
    tqsort_helper(base, m);
  if (nele-1 > m+1)
    tqsort_helper(base+m+1, nele-m-1);
  return NULL;
}

/* Many-threaded quicksort */
static void tqsort_helper(data_t *base, size_t nele) {
  if (verbose >= 2) {
    size_t l = global_index(base);
    size_t r = global_index(base+nele-1);
    printf("tqsort.  [%lu,%lu]\n", (printi_t) l, (printi_t) r);
  }
  if (nele <= nele_max_serial) {
    /* Use sequential sort */
    if (use_qsort_lib)
      qsort_lib(base, nele);
    else
      qsort_serial(base, nele);
    return;
  }
  sort_task_t *t = new_task(base, nele);
  spawn_task(tq, sort_thread, (void *) t);
}

void tqsort(data_t *base, size_t nele) {
  int max_tasks = 0;
  init_task(nele);
  global_base = base;
  global_end = global_base + nele - 1;
  tqsort_helper(base, nele);
  if (using_threads) {
    max_tasks = join_tasks(tq);
    free_task_queue(tq);
  }
  if (verbose >= 1)
    printf("Sorted %lu elements with max %d concurrent tasks\n",
	   (printi_t) nele, max_tasks);
}


