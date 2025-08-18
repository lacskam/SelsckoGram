#ifndef SELSCSERVPROCESSING_H
#define SELSCSERVPROCESSING_H
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <glib.h>
#include "../erproc/erproc.h"
#include "../proto/proto.h"
extern GHashTable *users;


void* handle_client(void* arg);

int push_connected_client(int fd);
int remove_disconnected_client(int id);

void broadcast( uint8_t *payload);

#endif // SELSCSERVPROCESSING_H
