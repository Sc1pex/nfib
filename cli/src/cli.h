#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    Naive = 0b01,
    Matrix = 0b10,
} CliImpl;
// Must be in the same order as CliImpl
static const char* const cli_impls[] = {
    "naive",
    "matrix",
};
static const size_t num_cli_impls = sizeof(cli_impls) / sizeof(char*);

typedef struct {
    // Inclusive range of numbers
    uint64_t min_num;
    uint64_t max_num;

    // Bitfield of implementations
    uint64_t impl;

    // Can be null if no output is specified
    char* csv_file_output;

    // Number of runs per number per implementaion
    int runs;

    // Number of threads for calculation
    int threads;
} Cli;

bool cli_parse(Cli* cli, int argc, char** argv);
void cli_free(Cli* cli);

bool cli_impl(const Cli* cli, CliImpl m);
