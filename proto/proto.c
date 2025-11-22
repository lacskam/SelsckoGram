#include "proto.h"
#include "../erproc/erproc.h"
#include "packet_type.h"
int build_packet(packet **pkt,uint8_t type, const char *message) {

    uint32_t len = strlen(message);
    *pkt = Malloc(sizeof(packet_header) + len);
    (*pkt)->header.navi_ids.source_id = 0;
    (*pkt)->header.navi_ids.dest_id   = 0;
    (*pkt)->header.type = type;
    (*pkt)->header.payload_size = len;
    memcpy((*pkt)->payload, message, len);

    return sizeof(packet_header) + len;
}


int parce_packet(int fd,uint8_t **payload) {
    packet_header header;
    ssize_t hnread=Read(fd,&header,sizeof(packet_header));

    if(hnread==0 || hnread==-2) {
        return -2; //client crashed or disconnected
    }
    *payload = Malloc(header.payload_size + 1);
    ssize_t nread=Read(fd,*payload,header.payload_size);
    if(nread==0 || nread==-2) {
        free(payload);
        return -2; //client crashed or disconnected
    }
    if(is_packet_type_valid(header.type)!=0)

        return -1;

    (*payload)[header.payload_size] = '\0';

    printf("[LOG] - src=%d dst=%d type=%s, message=%s - [parce_packet]\n",
           header.navi_ids.source_id,
           header.navi_ids.dest_id,
           get_str_type(header.type),
           *payload);

    return header.payload_size;
}


int send_packet(int fd, uint8_t type, const char* message) {

    packet *pkt;
    size_t total_size;

    total_size = build_packet(&pkt,type,message);
    if (!total_size) {
        perror("[ERROR] - Error to build packet - [send_packet]");
        free(pkt);
        return -1;
    }
    Write(fd,pkt,total_size);
    free(pkt);
    return 0;

}
