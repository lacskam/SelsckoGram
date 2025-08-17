#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "erproc/erproc.h"
#include "proto/proto.h"
#define STAUS_ACCEPTING -111

void* handle_client(void* arg) {

    int fd = *(int*)arg;
    free(arg);

    while(1) {

        uint8_t *payload = NULL;
        int nread = parce_packet(fd, &payload);


        if (!nread) {
            perror("error to parce packet");
            continue;
        }

        send_packet(fd,0x01,payload);

        if (strncmp(payload, "end", 3) == 0) {
            printf("received 'end'.\n");

            free(payload);
            break;
        }
        free(payload);

    }


    close(fd);
    printf("client discnected.\n");
    return NULL;

}

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




