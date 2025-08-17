#include "proto.h"
#include "erproc.h"


int send_packet(int fd, uint8_t type, const char* message) {

    uint32_t len = strlen(message);

    packet *pkt=Malloc(sizeof(packet)+len);


    pkt->type = type;
    pkt->payload_size = len;
    memcpy(pkt->payload, message, len);

    size_t total_size = sizeof(packet) + len;
    Write(fd,pkt,total_size);

    free(pkt);
    return 0;

}

