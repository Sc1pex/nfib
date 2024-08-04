#include <gtest/gtest.h>
#include <thread>
#include <vector>

extern "C" {
#include "sync/channel.h"
}

TEST(Channel, create_free) {
    Sender* tx;
    Reciver* rx;

    channel_new(&tx, &rx, 32);

    Sender* tx2 = channel_clone_sender(tx);

    channel_free_sender(&tx2);
    channel_free_sender(&tx);
    channel_free_reciver(&rx);
}

TEST(Channel, send_recv_few) {
    Sender* tx;
    Reciver* rx;

    channel_new(&tx, &rx, 32);

    int x = 10;
    channel_send(tx, &x);

    int* read;
    channel_recv(rx, (void**) &read);
    EXPECT_EQ(*read, 10);

    for (int i = 0; i < 10; i++) {
        double* x = (double*) malloc(sizeof(int));
        *x = (i + 6.) / 7.;
        channel_send(tx, x);
    }

    for (int i = 0; i < 10; i++) {
        double* x;
        channel_recv(rx, (void**) &x);
        EXPECT_EQ(*x, (i + 6.) / 7.);
        free(x);
    }

    channel_free_sender(&tx);
    channel_free_reciver(&rx);
}

TEST(Channel, dont_recv_with_no_senders) {
    Sender* tx;
    Reciver* rx;

    channel_new(&tx, &rx, 32);
    channel_free_sender(&tx);

    void* data;
    ASSERT_FALSE(channel_recv(rx, &data));

    channel_free_reciver(&rx);
}

TEST(Channel, multi_sender) {
    Sender* tx;
    Reciver* rx;

    channel_new(&tx, &rx, 32);

    std::vector<std::thread> send_threads;
    for (int i = 0; i < 10; i++) {
        Sender* local_tx = channel_clone_sender(tx);
        send_threads.emplace_back(
            [](int i, Sender* tx) {
                for (int j = 0; j < 10; j++) {
                    int* x = (int*) malloc(sizeof(int));
                    *x = i * 10 + j;
                    channel_send(tx, x);
                }

                channel_free_sender(&tx);
            },
            i, local_tx
        );
    }
    channel_free_sender(&tx);

    std::thread recv_thread([&rx]() {
        std::vector<int> numbers(100, 1);

        while (true) {
            int* read;
            if (!channel_recv(rx, (void**) &read)) {
                break;
            }

            ASSERT_LT(*read, 100);
            numbers[*read] = 0;

            free(read);
        }

        for (auto& el : numbers) {
            ASSERT_EQ(el, 0);
        }
    });

    for (auto& t : send_threads) {
        t.join();
    }
    recv_thread.join();

    channel_free_reciver(&rx);
}
