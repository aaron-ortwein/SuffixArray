#include <time.h>
#include "timeutils.h"

#define BILLION 1000000000

double time_delta(struct timespec begin, struct timespec end) {
    return end.tv_sec - begin.tv_sec + (double) (end.tv_nsec - begin.tv_nsec) / BILLION;
}