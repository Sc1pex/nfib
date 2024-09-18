#include <stdio.h>
#include <time.h>
#include "cli.h"
#include "impl.h"
#include "num.h"

double timespec_diff_ms(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) * 1000.0
           + (end->tv_nsec - start->tv_nsec) / 1000000.0;
}

typedef struct {
    double time;
    BigNum res;
} RunResult;

// returns number of ms it takes to run f
RunResult run(FibImpl f, uint64_t n) {
    RunResult r;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    r.res = f(n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    r.time = timespec_diff_ms(&start, &end);

    return r;
}

int main(int argc, char** argv) {
    Cli c;
    if (!cli_parse(&c, argc, argv)) {
        return 69;
    }

    RunResult r;
    switch (c.impl) {
    case Naive:
        r = run(fib_naive, c.num);
        break;
    case Matrix:
        r = run(fib_matrix, c.num);
        break;
    }

    bignum_print(r.res);
    printf(" %f", r.time);
    return 0;
}
