#include "ringbuf.h"
#include <stdlib.h>

RingBuffer ringbuf_new(int capacity) {
    RingBuffer rb;
    rb.capacity = capacity;
    rb.data = malloc(sizeof(void*) * capacity);
    rb.read_idx = 0;
    rb.write_idx = 0;
    return rb;
}

bool ringbuf_full(RingBuffer* rb) {
    return ((rb->write_idx + 1) % rb->capacity == rb->read_idx);
}

bool ringbuf_empty(RingBuffer* rb) {
    return (rb->read_idx == rb->write_idx);
}

bool ringbuf_write(RingBuffer* rb, void* el) {
    if (ringbuf_full(rb)) {
        return false;
    }

    rb->data[rb->write_idx] = el;

    rb->write_idx += 1;
    rb->write_idx %= rb->capacity;
    return true;
}

bool ringbuf_read(RingBuffer* rb, void** el) {
    if (ringbuf_empty(rb)) {
        return false;
    }

    *el = rb->data[rb->read_idx];

    rb->read_idx += 1;
    rb->read_idx %= rb->capacity;
    return true;
}

void ringbuf_free(RingBuffer* rb) {
    free(rb->data);
}
