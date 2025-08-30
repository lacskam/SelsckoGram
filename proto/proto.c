#include "proto.h"
#include "../erproc/erproc.h"

int build_packet(packet **pkt,uint8_t type, const char *message) {
    uint32_t len = strlen(message);
    *pkt = Malloc(sizeof(packet) + len);

    (*pkt)->type = type;
    (*pkt)->payload_size = len;
    memcpy((*pkt)->payload, message, len);

    return sizeof(packet) + len;
}


int parce_packet(int fd,uint8_t **payload) {


      packet header;
      Read(fd,&header,sizeof(packet));

      *payload = Malloc(header.payload_size + 1);
      Read(fd,*payload,header.payload_size);

      (*payload)[header.payload_size] = '\0';

      printf("type: 0x%02X, message: %s\n", header.type, *payload);

      return header.payload_size;
}

int send_packet(int fd, uint8_t type, const char* message) {

    packet *pkt;
    size_t total_size;

    total_size = build_packet(&pkt,type,message);
    if (!total_size) {
        perror("error to build packet");
        free(pkt);
        return -1;
    }


    Write(fd,pkt,total_size);
    free(pkt);
    return 0;

}

