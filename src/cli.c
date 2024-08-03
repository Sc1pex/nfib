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
    cli->min_num = atoi(range);
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

void default_val_range(Cli* cli) {
    cli->min_num = 1;
    cli->max_num = 1000;
}

static const CliOption commands[] = {
    {
        { "--output", "-o" },

        parse_output_filename,
        NULL,
        "    -o|--output <output_filename.csv>    Name of file to write timings to\n",
    },
    {
        { "--range" },

        parse_range,
        default_val_range,
        "    --range <from>..<to>    Range of fibonacci numbers to calculate\n"
        "                            (from must be <= to to)\n",
    },
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
            for (const char* const* flag = c->flags; *flag; flag++) {
                if (strcmp(*flag, argv[current_arg]) == 0) {
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
