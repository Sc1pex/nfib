#include <gtest/gtest.h>

extern "C" {
#include "fib_impl.h"
}

TEST(MatrixFib, upto100) {
    for (uint64_t i = 0; i <= 1000; i++) {
        BigNum f1 = fib_naive(i);
        BigNum f2 = fib_matrix(i);

        ASSERT_TRUE(bignum_eq(f1, f2));

        bignum_free(&f1);
        bignum_free(&f2);
    }
}
