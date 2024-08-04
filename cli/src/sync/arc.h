#pragma once

#include <stdatomic.h>

typedef struct {
    void* data;
    atomic_int* count;
} Arc;

Arc arc_new(void* data);
Arc arc_clone(Arc* arc);
void arc_free(Arc* arc, void (*cleanup)(void*));
