#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char* output_filename;
    uint64_t min_num;
    uint64_t max_num;
    int runs;
    int threads;
} Cli;

bool cli_parse(Cli* cli, int argc, char** argv);
void cli_free(Cli* cli);
