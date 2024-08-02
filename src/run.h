#pragma once

#include "fibb_impl.h"

// Stats about runs in ms
typedef struct {
    double min;
    double max;
    double avg;
    BigNum num;
} RunStats;

RunStats run(FibImpl impl, int n, int runs);
