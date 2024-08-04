#include <stdio.h>
#include <stdlib.h>
#include "assertf.h"
#include "cli.h"
#include "csv.h"
#include "fib_impl.h"
#include "run.h"

int main(int argc, char** argv) {
    Cli cli;
    if (!cli_parse(&cli, argc, argv)) {
        exit(1);
    }

    CSVOutput csv;
    assertf(csv_init(&csv, cli.output_filename), "failed to init csv file");

    for (int i = cli.min_num; i <= cli.max_num; i++) {
        RunStats stats = run(naive, i, cli.runs);
        char* num = bignum_string(stats.num);

        printf(
            "Generated %d'th fib number in %fms avg (%fms min, %fms max) across %d runs:\n%s\n",
            i, stats.avg, stats.min, stats.max, cli.runs, num
        );
        csv_write(&csv, &stats);

        bignum_free(&stats.num);
        free(num);
    }

    assertf(csv_close(&csv), "failed to clsoe csv file");
    cli_free(&cli);
    return 0;
}
