#include "selscservprocessing.h"

void* handle_client(void* arg) {

    int fd = *(int*)arg;
    free(arg);

    while(1) {
        uint8_t *payload = NULL;
        int nread = parce_packet(fd, &payload);
        if(nread==-2) {   //client crashed or disconnected
            free(payload);
            break;
        }
        if (!nread) {
            perror("[ERROR] - error to parce packet - [handle_client]");
            continue;
        }

        // send_packet(fd,0x01,payload);
        broadcast(payload);
        if (strncmp(payload, "end", 3) == 0) {
            printf("[LOG] - received 'end' - [handle_client]\n");
            free(payload);
            break;
        }
        free(payload);

    }
    close(fd);
    return NULL;

}


int push_connected_client(int fd) {
    if (fd == -1) {
        perror("[ERROR] - invalid file descriptor - [push_connected_client]\n");
        return -1;
    }

    static int id_count = 0;
    int id = id_count++;

    struct users *user;

    HASH_FIND_INT(selsc_users, &id, user);
    if (user==NULL) {
        user = (struct users *)Malloc(sizeof *user);
        user->client.user_fd = fd;
        user->client.user_id = id;
        HASH_ADD_INT(selsc_users,id,user);
        printf("[LOG] - client added - [push_connected client]\n");
    }
    else {
        perror("[ERROR] - user has already been created - [push_connected client]\n");
        return -2;
    }
    return id;
}



int remove_disconnected_client(int id) {

    return 0;
}




void broadcast(uint8_t *payload) {
    struct users *s;

    for (s = selsc_users; s != NULL; s = s->hh.next) {
        printf("[LOG] - user id %d: - [broadcast]\n", s->client.user_id);
        send_packet(s->client.user_fd,0x01,payload);
    }

}
