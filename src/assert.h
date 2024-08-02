#pragma once

#include <assert.h>
#include <stdio.h>

#define assert_error(M, ...) \
    fprintf(stderr, "Assert at %s:%d failed: " M "\n", __FILE__, __LINE__, \
            ##__VA_ARGS__)

#define assertf(A, M, ...) \
    if (!(A)) { \
        assert_error(M, ##__VA_ARGS__); \
        assert(A); \
    }
