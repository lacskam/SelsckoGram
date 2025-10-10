#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {

    uint8_t source_id;
    uint8_t dest_id;


} packet_navigation_info;

#pragma pack(push,1)

typedef struct {
    packet_navigation_info navi_ids;
    uint8_t  type;
    uint32_t payload_size;

} packet_header;

#pragma pack(pop)

typedef struct
{
    packet_header header;
    uint8_t payload[];
} packet;



int build_packet(packet **pkt,uint8_t type, const char *message);
int parce_packet(int fd, uint8_t **payload);
int send_packet(int fd, uint8_t type, const char* message);

#endif // PROTO_H
