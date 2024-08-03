#include <stdio.h>
#include <stdlib.h>
#include "cli.h"
#include "common.h"
#include "csv.h"
#include "fib_impl.h"
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
    Cli cli;
    if (!cli_parse(&cli, argc, argv)) {
        exit(1);
    }

    CSVOutput csv;
    assertf(csv_init(&csv, cli.output_filename), "failed to init csv file");

    // test_random_bignum(1000000);
    const int runs = 10;

    for (int i = cli.min_num; i <= cli.max_num; i++) {
        RunStats stats = run(naive, i, runs);
        char* num = bignum_string(stats.num);

        printf(
            "Generated %d'th fib number in %fms avg (%fms min, %fms max) across %d runs:\n%s\n",
            i, stats.avg, stats.min, stats.max, runs, num
        );
        csv_write(&csv, &stats);

        bignum_free(&stats.num);
        free(num);
    }

    assertf(csv_close(&csv), "failed to clsoe csv file");
    cli_free(&cli);
    return 0;
}
