#include "run.h"
#include <float.h>
#include <time.h>
#include <unistd.h>
#include "assertf.h"

double timespec_diff_ms(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) * 1000.0
           + (end->tv_nsec - start->tv_nsec) / 1000000.0;
}

void run(FibImpl impl, int n, int runs, RunStats* stats) {
    assertf(n >= 0, "n should be > 0, but is %d", n);
    assertf(runs > 0, "runs should be > 1, but is %d", runs);

    stats->n = n;
    stats->runs = runs;
    stats->avg = 0;
    stats->min = DBL_MAX;
    stats->max = DBL_MIN;

    for (int i = 0; i < runs; i++) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        BigNum res = impl(n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Make sure all results are the same
        if (i == 0) {
            stats->num = res;
        } else {
            assertf(
                bignum_eq(stats->num, res), "%s != %s",
                bignum_string(stats->num), bignum_string(res)
            );
            bignum_free(&res);
        }

        double time = timespec_diff_ms(&start, &end);
        stats->avg += time;
        stats->min = MIN(stats->min, time);
        stats->max = MAX(stats->max, time);
    }

    stats->avg /= runs;
}
