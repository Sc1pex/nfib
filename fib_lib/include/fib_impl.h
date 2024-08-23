#pragma once

#include <stdint.h>
#include "num.h"

typedef BigNum (*FibImpl)(uint64_t);

BigNum fib_naive(uint64_t num);
BigNum fib_matrix(uint64_t num);
