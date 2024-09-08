#include "cli.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool parse_range(Cli* cli, int* argc, char*** argv) {
    char* range = (*argv)[0];
    (*argc)--;
    (*argv)++;

    char* sep = strstr(range, "..");
    if (sep == NULL) {
        return false;
    }

    size_t digits = sep - range;
    for (size_t i = 0; i < digits; i++) {
        if (!isdigit(range[i])) {
            return false;
        }
    }
    cli->min_num = strtol(range, NULL, 10);
    range = sep + 2;
    digits = strlen(range);
    for (size_t i = 0; i < digits; i++) {
        if (!isdigit(range[i])) {
            return false;
        }
    }
    cli->max_num = strtol(range, NULL, 10);

    if (cli->min_num > cli->max_num) {
        return false;
    }

    return true;
}

bool parse_impl(Cli* cli, int* argc, char*** argv) {
    (*argc)--;
    char* range = (*argv)[0];

    char* p = strtok(range, ",");
    while (p != NULL) {
        bool ok = false;
        for (size_t i = 0; i < num_cli_impls; i++) {
            if (strcmp(p, cli_impls[i]) == 0) {
                ok = true;
                cli->impl |= (1 << i);
                break;
            }
        }
        if (!ok) {
            printf("Unknown implementation: %s\n", p);
            return false;
        }

        p = strtok(NULL, ",");
    }

    return true;
}

typedef bool (*ParseFn)(Cli* cli, int* argc, char*** argv);
typedef bool (*SetDefaultFn)(Cli* cli);

typedef struct {
    const char* name;
    const char* description;

    ParseFn parse;
} RequiredArg;

typedef struct {
    const char* flag;
    const char* description;

    ParseFn parse;
    SetDefaultFn set_default;
} OptionalArg;

static const RequiredArg required_args[] = {
    {
        "range",
        "Inclusive range of numbers to calculate in the form x..y (y must be greater or equal to x)",
        parse_range,
    },
    {
        "impl",
        "Array of implementations seperated by commas to use for each number. \n"
        "            Possible values are: naive,matrix",
        parse_impl,
    }
};
static const size_t num_required_args =
    sizeof(required_args) / sizeof(RequiredArg);
static const OptionalArg optional_args[] = {};
static const size_t num_optional_args =
    sizeof(optional_args) / sizeof(OptionalArg);

void print_usage() {
    printf("Usage: nfib ");
    for (size_t i = 0; i < num_required_args; i++) {
        printf("<%s> ", required_args[i].name);
    }
    printf("[Options]\n\n");

    printf("Required arguments:\n");
    for (size_t i = 0; i < num_required_args; i++) {
        printf(
            "    <%s>: %s\n", required_args[i].name,
            required_args[i].description
        );
    }
    printf("Optional arguments:\n");
    for (size_t i = 0; i < num_optional_args; i++) {
        printf(
            "    %s: %s\n", optional_args[i].flag, optional_args[i].description
        );
    }
}

bool parse_required(Cli* cli, int* argc, char*** argv) {
    if (*argc < num_required_args) {
        return false;
    }

    for (size_t i = 0; i < num_required_args; i++) {
        if (!required_args[i].parse(cli, argc, argv)) {
            return false;
        }
    }

    return true;
}

void init_optional(Cli* cli) {
    for (size_t i = 0; i < num_optional_args; i++) {
        optional_args[i].set_default(cli);
    }
}

bool parse_optional(Cli* cli, int* argc, char*** argv) {
    while (*argc) {
        bool good = false;
        for (size_t i = 0; i < num_optional_args; i++) {
            if (strcmp(optional_args[i].flag, (*argv)[0]) != 0) {
                continue;
            }

            if (!optional_args[i].parse(cli, argc, argv)) {
                return false;
            }
            good = true;
        }

        if (!good) {
            printf("Unknown option: %s\n", (*argv)[0]);
            return false;
        }
    }
    return true;
}

bool cli_parse(Cli* cli, int argc, char** argv) {
    init_optional(cli);

    // Skip first argument (name of binary)
    argv++;
    argc -= 1;

    if (!parse_required(cli, &argc, &argv)) {
        print_usage();
        return false;
    }

    if (!parse_optional(cli, &argc, &argv)) {
        print_usage();
        return false;
    }

    return true;
}

void cli_free(Cli* cli) {
    if (cli->csv_file_output != NULL) {
        free(cli->csv_file_output);
        cli->csv_file_output = NULL;
    }
}

bool cli_impl(const Cli* cli, CliImpl m) {
    return (cli->impl & m) != 0;
}
bool cli_output(const Cli* cli, CliOutput o) {
    return (cli->output & o) != 0;
}
