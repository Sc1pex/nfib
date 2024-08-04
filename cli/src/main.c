#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "assertf.h"
#include "cli.h"
#include "csv.h"
#include "fib_impl.h"
#include "run.h"

typedef struct {
    int start_num;
    int end_num;
    int increment_by;
    int runs;
} ThreadArg;

int run_thread(void* arg) {
    ThreadArg* t_arg = (ThreadArg*) arg;

    for (int i = t_arg->start_num; i <= t_arg->end_num;
         i += t_arg->increment_by) {
        RunStats stats = run(naive, i, t_arg->runs);
        char* num = bignum_string(stats.num);

        printf(
            "Generated %d'th fib number in %fms avg (%fms min, %fms max) across %d runs:\n%s\n",
            i, stats.avg, stats.min, stats.max, t_arg->runs, num
        );
        // csv_write(&csv, &stats);

        bignum_free(&stats.num);
        free(num);
    }

    free(t_arg);
    return 0;
}

int main(int argc, char** argv) {
    Cli cli;
    if (!cli_parse(&cli, argc, argv)) {
        exit(1);
    }

    CSVOutput csv;
    assertf(csv_init(&csv, cli.output_filename), "failed to init csv file");

    thrd_t* threads = malloc(sizeof(thrd_t) * cli.threads);
    for (int i = 0; i < cli.threads; i++) {
        ThreadArg* t_arg = malloc(sizeof(ThreadArg));
        t_arg->start_num = cli.min_num + i;
        t_arg->end_num = cli.max_num;
        t_arg->increment_by = cli.threads;
        t_arg->runs = cli.runs;
        assertf(
            thrd_create(&threads[i], &run_thread, t_arg) == 0,
            "failed to create thread"
        );
    }

    for (int i = 0; i < cli.threads; i++) {
        thrd_join(threads[i], NULL);
    }
    free(threads);
    assertf(csv_close(&csv), "failed to clsoe csv file");
    cli_free(&cli);
    return 0;
}
