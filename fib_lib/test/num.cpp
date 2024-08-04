#include <gtest/gtest.h>
#include <random>

extern "C" {
#include "num.h"
}

TEST(BigNum, random_sum) {
    int count = 10000000;
    std::mt19937 gen(time(nullptr));

    for (int i = 0; i < count; i++) {
        uint64_t n1 = gen();
        uint64_t n2 = gen();
        uint64_t nsum = n1 + n2;

        BigNum b1 = bignum_fromu64(n1);
        BigNum b2 = bignum_fromu64(n2);
        BigNum bsum = bignum_add(b1, b2);
        BigNum bsum1 = bignum_fromu64(nsum);

        ASSERT_TRUE(bignum_eq(bsum, bsum1));
    }
}
