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

#include "database/database.h"

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

    db_connect();
    // const char* login="asd";
    // char* salt=get_salt(login);
    // const char* password="asd";

    // char* hash_pas=hash_password(password,salt);
    // printf("pas:%s\nsalt:%s\nhashed_pas:%s\n",password,salt, hash_pas);
    const char* password="asd";
    char* login="asd1";
    verify_password(login,password);

    while (1) {
        puts("[LOG] - Waiting clients - [server]\n");
        int* client_fd =Malloc(sizeof(int));
        *client_fd=Accept(servSocket, (struct sockaddr *)&adr,&adrlen);

        puts("[LOG] - Client conected - [server]\n");

        pthread_t tid;

        if (pthread_create(&tid,NULL,handle_client,client_fd)!=0) {
            perror("[ERROR] - Pthread_create error - [server]");
            close(*client_fd);
            free(client_fd);
        } else {
            pthread_detach(tid);
        }
    }

    close(servSocket);

    return 0;
}
