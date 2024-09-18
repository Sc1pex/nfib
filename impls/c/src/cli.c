#include "cli.h"
#include <stdlib.h>
#include <string.h>

bool cli_parse(Cli* cli, int argc, char** argv) {
    if (argc != 3) {
        return false;
    }

    // First arg is the implementation
    char* impl = argv[1];

    bool ok_impl = false;
    for (size_t i = 0; i < num_cli_impls; i++) {
        if (strcmp(impl, cli_impls[i]) == 0) {
            cli->impl = i;
            ok_impl = true;
            break;
        }
    }
    if (!ok_impl) {
        return false;
    }

    // Second arg is number
    char* num = argv[2];
    cli->num = strtoul(num, NULL, 10);

    return true;
}
