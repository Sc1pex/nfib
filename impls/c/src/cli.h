#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    Naive,
    Matrix,
} CliImpl;
static const char* const cli_impls[] = {
    "naive",
    "matrix",
};
static const size_t num_cli_impls = sizeof(cli_impls) / sizeof(char*);

typedef struct {
    uint64_t num;
    CliImpl impl;
} Cli;

bool cli_parse(Cli* cli, int argc, char** argv);
