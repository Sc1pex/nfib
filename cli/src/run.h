#pragma once

#include "fib_impl.h"

// Stats about runs in ms
typedef struct {
    double min;
    double max;
    double avg;

    uint64_t n;
    int runs;

    BigNum num;
} RunStats;

void run(FibImpl impl, uint64_t n, int runs, RunStats* stats);
