#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H
#include <stdio.h>
#include <stdint.h>
#define PACKET_TYPE_MESSAGE    0x01
#define PACKET_TYPE_PING       0x03


const char* get_str_type(uint8_t type);
int is_packet_type_valid(uint8_t type);


#endif // PACKET_TYPE_H
