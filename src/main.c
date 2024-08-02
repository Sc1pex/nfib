#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "fibb_impl.h"
#include "run.h"

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
    const int runs = 10;

    for (int i = 0; i <= 5000; i++) {
        RunStats stats = run(naive, i, runs);
        printf(
            "Generated %d'th fibbn number in %fms avg (%f min, %f max)\n across %d runs",
            i, stats.avg, stats.min, stats.max, 10);
        bignum_free(&stats.num);
    }
    return 0;
}
