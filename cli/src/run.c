#include "run.h"
#include <float.h>
#include <time.h>
#include "assertf.h"

RunStats run(FibImpl impl, int n, int runs) {
    assertf(n >= 0, "n should be > 0, but is %d", n);
    assertf(runs > 0, "runs should be > 1, but is %d", runs);

    RunStats stats;
    stats.n = n;
    stats.avg = 0;
    stats.min = DBL_MAX;
    stats.max = DBL_MIN;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        BigNum res = impl(n);
        clock_t end = clock();

        // Make sure all results are the same
        if (i == 0) {
            stats.num = res;
        } else {
            assertf(
                bignum_eq(stats.num, res), "%s != %s", bignum_string(stats.num),
                bignum_string(res)
            );
            bignum_free(&res);
        }

        double time = ((double) end - start) / CLOCKS_PER_SEC * 1000.;
        stats.avg += time;
        stats.min = MIN(stats.min, time);
        stats.max = MAX(stats.max, time);
    }

    stats.avg /= runs;
    return stats;
}
