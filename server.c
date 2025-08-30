#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#include"include/uthash/include/uthash.h"

#include "erproc/erproc.h"
#include "proto/proto.h"
#define STAUS_ACCEPTING -111
#include "selscserv_processing/selscservprocessing.h"


struct users {
    int id;            /* we'll use this field as the key */
    int fd;
    UT_hash_handle hh; /* makes this structure hashable */
};


struct users *selsc_users = NULL;


int main() {




    int servSocket = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    Bind(servSocket,(struct sockaddr *)&adr,sizeof adr);

    Listen(servSocket,5);

    socklen_t adrlen = sizeof adr;
    int fd=STAUS_ACCEPTING;

    while (1) {
        printf("waiting clients.\n");
        int* client_fd = Malloc(sizeof(int));

        *client_fd = Accept(servSocket, (struct sockaddr *)&adr,&adrlen);

        printf("client conected.\n");
        push_connected_client(*client_fd);

      pthread_t tid;

      if (pthread_create(&tid,NULL,handle_client,client_fd)!=0) {
        perror("pthread_create error");
            close(*client_fd);
            free(client_fd);
      } else {
           pthread_detach(tid);
      }
    }

       close(servSocket);

      return 0;
}




