#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include "erproc.h"
#include <string.h>
#include "proto.h"

int main() {
    int fd =  Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};

    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    inet_pton(AF_INET,"127.0.0.1",&adr.sin_addr);
    Connect(fd,(struct sockaddr *) &adr,sizeof adr);
    while (1) {
        char message[256];
        if (!fgets(message, sizeof message, stdin))
            break;
        
        send_packet(fd,0x01,message);

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

            exit(EXIT_FAILURE);

        }

        if (nread == 0) {
            printf("end of file or client disconect\n");
            
        }
        printf("Тип: 0x%02X, Сообщение: %s\n", header.type, payload);


    }


    close(fd);

    return 0;
}
