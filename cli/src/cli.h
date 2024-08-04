#pragma once

#include <stdbool.h>

typedef struct {
    char* output_filename;
    int min_num;
    int max_num;
    int runs;
    int threads;
} Cli;

bool cli_parse(Cli* cli, int argc, char** argv);
void cli_free(Cli* cli);
