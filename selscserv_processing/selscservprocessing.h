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

#include"../include/uthash/include/uthash.h"


struct users {
    int id;            /* we'll use this field as the key */
    int fd;
    UT_hash_handle hh; /* makes this structure hashable */
};


extern struct users *selsc_users;



void* handle_client(void* arg);

int push_connected_client(int fd);
int remove_disconnected_client(int id);

void broadcast( uint8_t *payload);

#endif // SELSCSERVPROCESSING_H
