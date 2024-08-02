#include "fibb_impl.h"

BigNum naive(int num) {
    BigNum f1 = bignum_fromu64(0);
    BigNum f2 = bignum_fromu64(1);
    BigNum f3;

    while (num) {
        f3 = bignum_add(f1, f2);
        bignum_free(&f2);
        f2 = f1;
        f1 = f3;

        num -= 1;
    }
    bignum_free(&f2);
    return f1;
}
