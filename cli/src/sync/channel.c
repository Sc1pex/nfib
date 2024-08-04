#include "channel.h"
#include <stdlib.h>
#include <threads.h>
#include "arc.h"
#include "assertf.h"
#include "ringbuf.h"

typedef struct {
    mtx_t mutex;
    RingBuffer buff;

    cnd_t has_data;
    cnd_t has_space;

    int senders;
} Inner;

struct sender {
    Arc inner;
};

struct reciver {
    Arc inner;
};

void channel_new(Sender** tx_p, Reciver** rx_p, int capacity) {
    *tx_p = malloc(sizeof(Sender));
    *rx_p = malloc(sizeof(Reciver));
    Sender* tx = *tx_p;
    Reciver* rx = *rx_p;

    Inner* inner = malloc(sizeof(Inner));
    assertf(
        mtx_init(&inner->mutex, mtx_plain) == thrd_success,
        "failed to initialize mutex"
    );
    assertf(
        cnd_init(&inner->has_data) == thrd_success,
        "failed to initialize condvar has_data"
    );
    assertf(
        cnd_init(&inner->has_space) == thrd_success,
        "failed to initialize condvar has_space"
    );
    inner->buff = ringbuf_new(capacity);
    inner->senders = 1;

    Arc inner_arc = arc_new(inner);

    tx->inner = arc_clone(&inner_arc);
    rx->inner = inner_arc;
}

Sender* channel_clone_sender(Sender* tx) {
    Sender* s = malloc(sizeof(Sender));
    s->inner = arc_clone(&tx->inner);

    Inner* inner = (Inner*) tx->inner.data;
    assertf(mtx_lock(&inner->mutex) == thrd_success, "failed to lock mutex");
    inner->senders += 1;
    assertf(
        mtx_unlock(&inner->mutex) == thrd_success, "failed to unlock mutex"
    );

    return s;
}

void channel_send(Sender* tx, void* data) {
    Inner* inner = (Inner*) tx->inner.data;
    assertf(mtx_lock(&inner->mutex) == thrd_success, "failed to lock mutex");
    while (1) {
        if (ringbuf_write(&inner->buff, data)) {
            cnd_signal(&inner->has_data);

            assertf(
                mtx_unlock(&inner->mutex) == thrd_success,
                "failed to unlock mutex"
            );
            return;
        }

        assertf(
            cnd_wait(&inner->has_space, &inner->mutex) == thrd_success,
            "failed to wait for has_space"
        );
    }
}

bool channel_recv(Reciver* rx, void** data) {
    Inner* inner = (Inner*) rx->inner.data;
    assertf(mtx_lock(&inner->mutex) == thrd_success, "failed to lock mutex");

    while (1) {
        void* d;
        if (ringbuf_read(&inner->buff, &d)) {
            *data = d;

            // If there are threads waiting to send a signal wake one of them up
            //
            // This dosen't use cnd_broadcast because if a thread is waiting for
            // this condition it means that the buffer is full. After a read one
            // space is avabile, so only one thread should be waked
            assertf(
                cnd_signal(&inner->has_space) == thrd_success,
                "failed to notify about has_space"
            );

            assertf(
                mtx_unlock(&inner->mutex) == thrd_success,
                "failed to unlock mutex"
            );
            return true;
        }

        // No senders left, channel can never receive
        if (inner->senders == 0) {
            return false;
        }

        assertf(
            cnd_wait(&inner->has_data, &inner->mutex) == thrd_success,
            "failed to wait for has_data"
        );
    }
}

static void free_inner(void* inner) {
    Inner* i = (Inner*) inner;
    ringbuf_free(&i->buff);

    mtx_destroy(&i->mutex);
    cnd_destroy(&i->has_space);
    cnd_destroy(&i->has_data);
}

void channel_free_sender(Sender** tx) {
    Inner* inner = (Inner*) (*tx)->inner.data;
    assertf(mtx_lock(&inner->mutex) == thrd_success, "failed to lock mutex");
    inner->senders -= 1;
    assertf(
        mtx_unlock(&inner->mutex) == thrd_success, "failed to unlock mutex"
    );

    arc_free(&(*tx)->inner, free_inner);
    free(*tx);
    *tx = NULL;
}

void channel_free_reciver(Reciver** rx) {
    arc_free(&(*rx)->inner, free_inner);
    free(*rx);
    *rx = NULL;
}
