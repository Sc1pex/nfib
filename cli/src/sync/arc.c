#include "arc.h"
#include <stdlib.h>

Arc arc_new(void* data) {
    Arc arc;
    arc.count = malloc(sizeof(atomic_int));
    *arc.count = 1;
    arc.data = data;
    return arc;
}

Arc arc_clone(Arc* arc) {
    Arc n_arc;

    n_arc.data = arc->data;
    n_arc.count = arc->count;
    *n_arc.count += 1;

    return n_arc;
}

void arc_free(Arc* arc, void (*cleanup)(void*)) {
    if (*arc->count == 1) {
        if (cleanup) {
            cleanup(arc->data);
        }
        free(arc->data);
        free(arc->count);
    } else {
        *arc->count -= 1;
    }
}
