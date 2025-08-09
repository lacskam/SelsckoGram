#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "erproc.h"
#include "proto.h"
#define STAUS_ACCEPTING -111

void* handle_client(void* arg) {

    int fd = *(int*)arg;
    free(arg);

    while(1) {
        ssize_t nread;

        packet header;
        nread = read(fd,&header,sizeof(packet));

        if (nread == -1) {
            perror("read error");

            exit(EXIT_FAILURE);

        }

        uint8_t* payload = malloc(header.payload_size + 1);
        nread = read(fd,payload,header.payload_size);
        if (nread == -1) {
            perror("read error");
             free(payload);
            exit(EXIT_FAILURE);

        }

        if (nread == 0) {
            printf("end of file or client disconect\n");
             free(payload);
            fd=STAUS_ACCEPTING;
        }
        printf("type: 0x%02X, message: %s\n", header.type, payload);

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
        int* client_fd = malloc(sizeof(int));

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




