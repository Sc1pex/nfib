#include <gtest/gtest.h>

extern "C" {
#include "math.h"
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

TEST(Mat2, exponent) {
    auto m = mat_fromu64(1, 2, 3, 4);

    auto m17 = mat_exp(&m, 17);
    auto m17_1 =
        mat_fromu64(617852597821, 900475124662, 1350712686993, 1968565284814);

    for (int i = 0; i < 4; i++) {
        ASSERT_TRUE(bignum_eq(m17.el[i], m17_1.el[i]));
    }

    mat_free(&m);
    mat_free(&m17);
    mat_free(&m17_1);
}
