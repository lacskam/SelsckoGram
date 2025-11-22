 #include "packet_type.h"

const char* get_str_type(uint8_t type)
{
    switch(type)
    {
    case PACKET_TYPE_MESSAGE: return "message";
    case PACKET_TYPE_PING: return "ping";
    default:
    {
        puts("[ERROR] - Unknown packet type - [get_str_type]");
        return "";
    }

    }

}
int is_packet_type_valid(uint8_t type)
{
    switch(type) {
        case PACKET_TYPE_MESSAGE:
        case PACKET_TYPE_PING:
            return 0;
        default:
            return -1;
    }
}
