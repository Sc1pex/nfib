#pragma once

#include <stdint.h>
#include "num.h"

typedef BigNum (*FibImpl)(int);

BigNum naive(int num);
