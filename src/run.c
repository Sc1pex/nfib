#include "run.h"
#include <float.h>
#include <time.h>
#include "common.h"

RunStats run(FibImpl impl, int n, int runs) {
    assertf(n >= 0, "n should be > 0, but is %d", n);
    assertf(runs > 0, "runs should be > 1, but is %d", runs);

    RunStats stats;
    stats.avg = 0;
    stats.min = DBL_MAX;
    stats.max = DBL_MIN;

    BigNum first;

    for (int i = 0; i < runs; i++) {
        clock_t start = clock();
        BigNum res = impl(n);
        clock_t end = clock();

        // Make sure all results are the same
        if (i == 0) {
            first = res;
        } else {
            assertf(
                first.size == res.size,
                "runs gave different results. expected size of %zu, got %zu",
                first.size, res.size);
            for (size_t i = 0; i < first.size; i++) {
                assertf(first.digits[i] == res.digits[i],
                        "digit %zu is different. expected %d, got %d", i,
                        first.digits[i], res.digits[i])
            }
            bignum_free(&res);
        }

        double time = ((double) end - start) / CLOCKS_PER_SEC * 1000.;
        stats.avg += time;
        stats.min = MIN(stats.min, time);
        stats.max = MAX(stats.max, time);
    }

    bignum_free(&first);

    stats.avg /= runs;
    return stats;
}
