#include <gtest/gtest.h>
#include <random>

extern "C" {
#include "math.h"
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
        BigNum bsum = bignum_add(&b1, &b2);
        BigNum bsum1 = bignum_fromu64(nsum);

        ASSERT_TRUE(bignum_eq(bsum, bsum1));

        bignum_free(&b1);
        bignum_free(&b2);
        bignum_free(&bsum);
        bignum_free(&bsum1);
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
        BigNum bprod = bignum_mult(&b1, &b2);
        BigNum bsum1 = bignum_fromu64(nprod);

        ASSERT_TRUE(bignum_eq(bprod, bsum1));

        bignum_free(&b1);
        bignum_free(&b2);
        bignum_free(&bprod);
        bignum_free(&bsum1);
    }
}

TEST(Mat2, mult) {
    auto m1 = mat_fromu64(1, 3, 5, 2);
    auto m2 = mat_fromu64(3, 0, 2, 1);

    auto mp = mat_mat_mult(&m1, &m2);
    auto mp1 = mat_fromu64(9, 3, 19, 2);

    for (int i = 0; i < 4; i++) {
        ASSERT_TRUE(bignum_eq(mp.el[i], mp1.el[i]));
    }

    mat_free(&m1);
    mat_free(&m2);
    mat_free(&mp);
    mat_free(&mp1);
}

TEST(Mat2, vec_mult) {
    auto m = mat_fromu64(3, 0, 2, 1);
    auto v = vec_fromu64(4, 8);

    auto vp = mat_vec_mult(&m, &v);
    auto vp1 = vec_fromu64(12, 16);

    ASSERT_TRUE(bignum_eq(vp.x, vp1.x));

    mat_free(&m);
    vec_free(&v);
    vec_free(&vp);
    vec_free(&vp1);
}
