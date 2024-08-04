#pragma once

#include <stdbool.h>

typedef struct sender Sender;
typedef struct reciver Reciver;

// Creates a multi producer single consumer channel with given capacity
void channel_new(Sender** tx, Reciver** rx, int capacity);
Sender* channel_clone_sender(Sender* tx);

// Send data. Will block if there are `capacity` messages waiting to be recived
void channel_send(Sender* tx, void* data);
// Read data. Will block if there is no data
bool channel_recv(Reciver* rx, void** data);

void channel_free_sender(Sender** tx);
void channel_free_reciver(Reciver** rx);
