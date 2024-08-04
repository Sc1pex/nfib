#include "csv.h"
#include <stdlib.h>

bool csv_init(CSVOutput* out, const char* filename) {
    out->file = fopen(filename, "w");
    if (out->file == NULL) {
        return false;
    }

    // Write header
    fprintf(out->file, "idx,avg,min,max,num\n");

    return true;
}

void csv_write(CSVOutput* csv, const RunStats* run) {
    char* num = bignum_string(run->num);
    fprintf(csv->file, "%d,%f,%f,%f,%s\n", run->n, run->avg, run->min, run->max,
            num);
    free(num);
}

bool csv_close(CSVOutput* csv) {
    return fclose(csv->file) == 0;
}
