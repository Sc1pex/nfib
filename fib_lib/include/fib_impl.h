#pragma once

#include <stdint.h>
#include "num.h"

typedef BigNum (*FibImpl)(uint64_t);

BigNum naive(uint64_t num);
