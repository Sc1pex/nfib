#include <gtest/gtest.h>

extern "C" {
#include "sync/ringbuf.h"
}

TEST(RingBuf, create_free) {
    RingBuffer rb = ringbuf_new(32);

    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_FALSE(ringbuf_full(&rb));

    ringbuf_free(&rb);
}

TEST(RingBuf, push_read) {
    RingBuffer rb = ringbuf_new(32);

    int x = 10;
    ASSERT_TRUE(ringbuf_write(&rb, &x));

    int* read;
    ASSERT_TRUE(ringbuf_read(&rb, (void**) &read));
    EXPECT_EQ(*read, 10);

    for (int i = 0; i < 10; i++) {
        int* x = (int*) malloc(sizeof(int));
        *x = i * 10;
        ASSERT_TRUE(ringbuf_write(&rb, x));
    }

    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(ringbuf_read(&rb, (void**) &read));
        EXPECT_EQ(*read, i * 10);
        free(read);
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < i % 4; j++) {
            int* x = (int*) malloc(sizeof(int));
            *x = i * 10 + j;
            ASSERT_TRUE(ringbuf_write(&rb, x));
        }

        for (int j = 0; j < i % 4; j++) {
            ASSERT_TRUE(ringbuf_read(&rb, (void**) &read));
            EXPECT_EQ(*read, i * 10 + j);
            free(read);
        }
    }

    ringbuf_free(&rb);
}
