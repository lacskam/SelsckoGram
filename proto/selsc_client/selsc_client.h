#ifndef SELSC_CLIENT_H
#define SELSC_CLIENT_H

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>



typedef struct {
  uint32_t user_id;
  int user_fd;
  pthread_t user_tid;
} selsc_client;




#endif
