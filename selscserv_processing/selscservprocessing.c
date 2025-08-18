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

    int *fd_ptr = malloc(sizeof(int));
    *fd_ptr = fd;

    g_hash_table_insert(users, GINT_TO_POINTER(id), fd_ptr); // ключ — int, значение — int*

    return id;
}



int remove_disconnected_client(int id) {
    g_hash_table_remove(users, GINT_TO_POINTER(id));
    return 0;
}




void broadcast(uint8_t *payload) {
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, users);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
      int fd = GPOINTER_TO_INT(value);
       send_packet(fd,0x01,payload);
    }

}
