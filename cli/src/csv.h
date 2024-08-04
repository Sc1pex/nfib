#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "run.h"

typedef struct {
    FILE* file;
} CSVOutput;

bool csv_init(CSVOutput* out, const char* filename);
void csv_write(CSVOutput* csv, const RunStats* run);
bool csv_close(CSVOutput* csv);
