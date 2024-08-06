#pragma once

#include "fib_impl.h"

// Stats about runs in ms
typedef struct {
    double min;
    double max;
    double avg;

    int n;
    int runs;

    BigNum num;
} RunStats;

void run(FibImpl impl, int n, int runs, RunStats* stats);
