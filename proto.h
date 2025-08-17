#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma pack(push,1)
typedef struct
{
    uint8_t  type;
    uint32_t payload_size;
    uint8_t payload[];
} packet;


#pragma pack(pop)

int send_packet(int fd, uint8_t type, const char* message);

#endif // PROTO_H
