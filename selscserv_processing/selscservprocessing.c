#include "selscservprocessing.h"




void* handle_client(void* arg) {

    int fd = *(int*)arg;
    free(arg);

    while(1) {

        uint8_t *payload = NULL;
        int nread = parce_packet(fd, &payload);


        if (!nread) {
            perror("error to parce packet");
            continue;
        }

        // send_packet(fd,0x01,payload);
        broadcast(payload);
        if (strncmp(payload, "end", 3) == 0) {
            printf("received 'end'.\n");

            free(payload);
            break;
        }
        free(payload);

    }


    close(fd);
    printf("client discnected.\n");
    return NULL;

}


int push_connected_client(int fd) {

    static int id_count = 0;
    int id = id_count++;

    printf("client 1.\n");

    printf("client 2.\n");
    struct users *user;

    HASH_FIND_INT(selsc_users, &id, user);
    if (user==NULL) {
      user = (struct users *)malloc(sizeof *user);

      user->fd = fd;
      user->id = id;

      HASH_ADD_INT(selsc_users,id,user);
         printf("client added.\n");
    }

    printf("client 3.\n");
    return id;
}



int remove_disconnected_client(int id) {

    return 0;
}




void broadcast(uint8_t *payload) {

    struct users *s;


     for (s = selsc_users; s != NULL; s = s->hh.next) {
            printf("user id %d:\n", s->id);

          send_packet(s->fd,0x01,payload);
        }


}
