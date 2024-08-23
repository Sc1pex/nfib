#include "fib_impl.h"
#include "math.h"

BigNum fib_naive(uint64_t num) {
    BigNum f1 = bignum_fromu64(0);
    BigNum f2 = bignum_fromu64(1);
    BigNum f3;

    while (num) {
        f3 = bignum_add(&f1, &f2);
        bignum_free(&f2);
        f2 = f1;
        f1 = f3;

        num -= 1;
    }
    bignum_free(&f2);
    return f1;
}

BigNum fib_matrix(uint64_t num) {
    Mat2 m = mat_fromu64(0, 1, 1, 1);
    Mat2 me = mat_exp(&m, num);
    mat_free(&m);

    Vec2 f01 = vec_fromu64(0, 1);
    Vec2 res = mat_vec_mult(&me, &f01);

    mat_free(&me);
    vec_free(&f01);
    bignum_free(&res.y);

    return res.x;
}
