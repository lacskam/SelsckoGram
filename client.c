#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include "erproc/erproc.h"
#include <string.h>
#include "proto/proto.h"
#include <pthread.h>

void* listenning(void* arg) {
    int fd = *(int*)arg;
    free(arg);

    while (1) {
        ssize_t nread;
        packet header;

        nread = read(fd, &header, sizeof(packet));
        if (nread == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        uint8_t* payload = malloc(header.header.payload_size + 1);

        nread = read(fd, payload, header.header.payload_size);
        if (nread == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        payload[header.header.payload_size] = '\0';

        if (nread == 0) {
            printf("end of file or client disconnect\n");
        }
        if (header.header.type!=0x04)
        printf("Тип: 0x%02X, Сообщение: %s\n", header.header.type, payload);

        free(payload);
    }

    return NULL;
}

int main() {
    int fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};

    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);

    Connect(fd, (struct sockaddr*)&adr, sizeof adr);

    pthread_t tid;

    int *fd_ptr = malloc(sizeof(int));
    *fd_ptr = fd;

    if (pthread_create(&tid, NULL, listenning, fd_ptr) != 0) {
        perror("[ERROR] - Pthread_create error - [server]");
    } else {
        pthread_detach(tid);
    }

    while (1) {
        char message[256];
        if (!fgets(message, sizeof message, stdin))
            break;

        send_packet(fd, 0x01, message);
    }

    close(fd);
    return 0;
}
