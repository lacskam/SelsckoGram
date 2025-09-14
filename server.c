#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include"uthash.h"
#include "erproc/erproc.h"
#include "proto/proto.h"
#define STAUS_ACCEPTING -111
#include "selscserv_processing/selscservprocessing.h"

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
        printf("[LOG] - waiting clients - [server]\n");
        int client_fd = Accept(servSocket, (struct sockaddr *)&adr,&adrlen);

        printf("[LOG] - client conected - [server]\n");

        int* client_id=Malloc(sizeof(int));
        *client_id=push_connected_client(client_fd);


        pthread_t tid;

        if (pthread_create(&tid,NULL,handle_client,client_id)!=0) {
            perror("[LOG] - pthread_create error - [server]");
            remove_disconnected_client(client_id);
            close(client_fd);
            free(client_id);
        } else {
            pthread_detach(tid);
        }
    }

    close(servSocket);

    return 0;
}




