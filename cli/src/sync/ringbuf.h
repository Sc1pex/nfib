#pragma once

#include <stdbool.h>

typedef struct {
    int capacity;
    void** data;
    int read_idx;
    int write_idx;
} RingBuffer;

RingBuffer ringbuf_new(int capacity);
void ringbuf_free(RingBuffer* rb);

bool ringbuf_full(RingBuffer* rb);
bool ringbuf_empty(RingBuffer* rb);
bool ringbuf_write(RingBuffer* rb, void* el);
bool ringbuf_read(RingBuffer* rb, void** el);
