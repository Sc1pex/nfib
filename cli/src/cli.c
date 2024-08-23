#include "cli.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef bool (*parse)(Cli*, int*, char**);
typedef void (*set_default_values)(Cli*);

// Struct representing an option that can be configured from cli args
typedef struct {
    const char* flags[2];
    const int flag_count;

    parse parse_fn;
    // If this is NULL it is implied that this option is required
    set_default_values default_val_fn;

    const char* usage;
} CliOption;

bool parse_output_filename(Cli* cli, int* current_arg, char** argv) {
    int len = strlen(argv[*current_arg]);
    cli->output_filename = malloc(len);
    strcpy(cli->output_filename, argv[*current_arg]);
    (*current_arg)++;
    return true;
}

bool parse_range(Cli* cli, int* current_arg, char** argv) {
    const char* range = argv[*current_arg];
    (*current_arg)++;

    char* sep = strstr(range, "..");
    if (!sep) {
        return false;
    }
    int num_len = sep - range;
    for (int i = 0; i < num_len; i++) {
        if (range[i] < '0' || range[i] > '9') {
            return false;
        }
    }
    cli->min_num = strtoull(range, NULL, 10);
    range = sep + 2;
    num_len = strlen(range);
    for (int i = 0; i < num_len; i++) {
        if (range[i] < '0' || range[i] > '9') {
            return false;
        }
    }
    cli->max_num = atoi(range);

    if (cli->min_num > cli->max_num) {
        return false;
    }

    return true;
}

bool parse_runs(Cli* cli, int* current_arg, char** argv) {
    char* runs = argv[*current_arg];
    (*current_arg)++;

    int len = strlen(runs);
    for (int i = 0; i < len; i++) {
        if (runs[i] < '0' || runs[i] > '9') {
            return false;
        }
    }
    cli->runs = atoi(runs);

    return true;
}

bool parse_threads(Cli* cli, int* current_arg, char** argv) {
    char* threads = argv[*current_arg];
    (*current_arg)++;

    int len = strlen(threads);
    for (int i = 0; i < len; i++) {
        if (threads[i] < '0' || threads[i] > '9') {
            return false;
        }
    }
    cli->threads = atoi(threads);

    return true;
}

void default_val_range(Cli* cli) {
    cli->min_num = 1;
    cli->max_num = 1000;
}

void default_runs(Cli* cli) {
    cli->runs = 5;
}

void default_threads(Cli* cli) {
    // My cpu has 6 p cores so run on 12 threads
    cli->threads = 12;
}

static const CliOption commands[] = {
    {
        { "--output", "-o" },
        2,

        parse_output_filename,
        NULL,
        "    -o|--output <output_filename.csv>    Name of file to write timings to\n",
    },
    {
        { "--range", "-R" },
        2,

        parse_range,
        default_val_range,
        "    -R|--range <from>..<to>    Range of fibonacci numbers to calculate\n"
        "                            (from must be <= to to)\n",
    },
    {
        { "--runs", "-r" },
        2,

        parse_runs,
        default_runs,
        "    -r|--runs <runs>    Number of runs to do for each number",
    },
    {
        { "--threads", "-t" },
        2,

        parse_threads,
        default_threads,
        "    -t|--threads <threads>    Number of threads to use for measuring in parallel",
    }
};
static const int num_commands = sizeof(commands) / sizeof(CliOption);

void usage() {
    printf("Usage:    nfib [options]\nOptions:\n");
    for (int i = 0; i < num_commands; i++) {
        printf("%s\n", commands[i].usage);
    }
}

bool cli_parse(Cli* cli, int argc, char** argv) {
    bool* option_given = malloc(num_commands * sizeof(bool));

    for (int i = 0; i < num_commands; i++) {
        if (commands[i].default_val_fn) {
            commands[i].default_val_fn(cli);
        }
    }

    int current_arg = 1;
    while (current_arg < argc) {
        bool handled = false;

        for (int i = 0; i < num_commands && !handled; i++) {
            const CliOption* c = &commands[i];
            for (int f = 0; f < c->flag_count; f++) {
                if (strcmp(c->flags[f], argv[current_arg]) == 0) {
                    current_arg++;

                    if (!c->parse_fn(cli, &current_arg, argv)) {
                        printf("%s", commands[i].usage);
                        return false;
                    }

                    handled = true;
                    option_given[i] = true;
                    break;
                }
            }
        }

        if (!handled) {
            printf("Unknown argument: %s\n", argv[current_arg]);
            usage();
            return false;
        }
    }

    for (int i = 0; i < num_commands; i++) {
        if (!commands[i].default_val_fn && !option_given[i]) {
            printf("Expected argument\n%s", commands[i].usage);
            return false;
        }
    }
    free(option_given);

    return true;
}

void cli_free(Cli* cli) {
    free(cli->output_filename);
}
