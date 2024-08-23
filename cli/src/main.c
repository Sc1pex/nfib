#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include "assertf.h"
#include "cli.h"
#include "csv.h"
#include "fib_impl.h"
#include "run.h"
#include "sync/channel.h"

typedef struct {
    uint64_t start_num;
    uint64_t end_num;
    int increment_by;
    int runs;

    Sender* tx;
} ThreadArg;

int run_thread(void* arg) {
    ThreadArg* t_arg = (ThreadArg*) arg;

    for (uint64_t i = t_arg->start_num; i <= t_arg->end_num;
         i += t_arg->increment_by) {
        RunStats* stats = malloc(sizeof(RunStats));
        run(naive, i, t_arg->runs, stats);

        channel_send(t_arg->tx, stats);
    }

    channel_free_sender(&t_arg->tx);
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

    Sender* tx;
    Reciver* rx;
    channel_new(&tx, &rx, 32);

    thrd_t* threads = malloc(sizeof(thrd_t) * cli.threads);
    for (int i = 0; i < cli.threads; i++) {
        ThreadArg* t_arg = malloc(sizeof(ThreadArg));
        t_arg->start_num = cli.min_num + i;
        t_arg->end_num = cli.max_num;
        t_arg->increment_by = cli.threads;
        t_arg->runs = cli.runs;
        t_arg->tx = channel_clone_sender(tx);
        assertf(
            thrd_create(&threads[i], &run_thread, t_arg) == 0,
            "failed to create thread"
        );
    }
    channel_free_sender(&tx);

    RunStats* stats;
    while (channel_recv(rx, (void**) &stats)) {
        printf(
            "Generated %lu'th fib number in %fms avg (%fms min, %fms max) across %d runs:\n",
            stats->n, stats->avg, stats->min, stats->max, stats->runs
        );
        csv_write(&csv, stats);
        bignum_free(&stats->num);
        free(stats);
    }
    channel_free_reciver(&rx);

    for (int i = 0; i < cli.threads; i++) {
        thrd_join(threads[i], NULL);
    }
    free(threads);
    assertf(csv_close(&csv), "failed to clsoe csv file");
    cli_free(&cli);
    return 0;
}
