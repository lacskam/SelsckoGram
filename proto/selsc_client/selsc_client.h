#ifndef SELSC_CLIENT_H
#define SELSC_CLIENT_H

#include <sndint.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


typedef struct {
  uint32_t user_id;
  int user_fd;
  pthread_t user_tid;
} client;




#endif
