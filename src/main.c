#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "fibb_impl.h"
#include "num.h"

void test_random_bignum(int count) {
    for (int i = 0; i < count; i++) {
        uint64_t n1 = rand();
        uint64_t n2 = rand();
        uint64_t nsum = n1 + n2;

        BigNum b1 = bignum_fromu64(n1);
        BigNum b2 = bignum_fromu64(n2);
        BigNum bsum = bignum_add(b1, b2);
        BigNum bsum1 = bignum_fromu64(nsum);

        assert(bsum.size == bsum1.size);
        for (int j = 0; j < bsum.size; j++) {
            assert(bsum.digits[j] == bsum1.digits[j]);
        }
    }
}

int main(int argc, char** argv) {
    // test_random_bignum(1000000);

    for (int i = 0; i < 4000; i++) {
        // printf("%d fibb is ", i);
        BigNum f = naive(i);
        bignum_print(f);
        printf("\n");
    }
    return 0;
}
