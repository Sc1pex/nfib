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

        bignum_free(&b1);
        bignum_free(&b2);
        bignum_free(&bsum);
        bignum_free(&bsum1);
    }
}

TEST(BigNum, multiply) {
    for (uint64_t i = 0; i < 100000; i++) {
        for (uint64_t j = 0; j < 100000; j++) {
            BigNum b1 = bignum_fromu64(i);
            BigNum b2 = bignum_fromu64(j);

            BigNum bprod = bignum_mult(b1, b2);
            BigNum bprod1 = bignum_fromu64(i * j);

            ASSERT_TRUE(bignum_eq(bprod, bprod1))
                << i << "*" << j << " should be: " << i * j
                << ".\nGot: " << bignum_string(bprod);

            bignum_free(&b1);
            bignum_free(&b2);
            bignum_free(&bprod);
            bignum_free(&bprod1);
        }
    }
}

TEST(BigNum, random_mult) {
    int count = 10000000;
    std::mt19937 gen(time(nullptr));

    for (int i = 0; i < count; i++) {
        uint64_t n1 = gen();
        uint64_t n2 = gen();
        uint64_t nprod = n1 * n2;

        BigNum b1 = bignum_fromu64(n1);
        BigNum b2 = bignum_fromu64(n2);
        BigNum bprod = bignum_mult(b1, b2);
        BigNum bsum1 = bignum_fromu64(nprod);

        ASSERT_TRUE(bignum_eq(bprod, bsum1));

        bignum_free(&b1);
        bignum_free(&b2);
        bignum_free(&bprod);
        bignum_free(&bsum1);
    }
}
