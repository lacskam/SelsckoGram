#include "selscservprocessing.h"

pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;


void* handle_client(void* arg) {
    usleep(1000);
    int id = *(int*)arg;
    int fd=get_user_fd(id);
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
    remove_disconnected_client(id);
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

    pthread_mutex_lock(&users_mutex);
    struct users *user;

    HASH_FIND_INT(selsc_users, &id, user);
    if (user==NULL) {
        user = (struct users *)Malloc(sizeof *user);
        user->id=id;
        user->client.user_fd = fd;
        user->client.user_id = id;
        HASH_ADD_INT(selsc_users,id,user);
        printf("[LOG] - client added - [push_connected client]\n");
    }
    else {
        perror("[ERROR] - user has already been created - [push_connected client]\n");
        pthread_mutex_unlock(&users_mutex);
        return -2;
    }
    pthread_mutex_unlock(&users_mutex);
    return id;
}



int remove_disconnected_client(int id) {
    pthread_mutex_lock(&users_mutex);
    struct users *user;
    HASH_FIND_INT(selsc_users,&id,user);
    if(user!=NULL) {
        HASH_DEL(selsc_users,user);
        free(user);
        pthread_mutex_unlock(&users_mutex);
        printf("[LOG] - Client (id: %d) removed - [push_connected_client]\n", id);
        return 0;
    }
    pthread_mutex_unlock(&users_mutex);
    printf("[ERROR] - Client was not found - [push_connected_client]\n");
    return -1;
}




void broadcast(uint8_t *payload) {
    pthread_mutex_lock(&users_mutex);
    struct users *s;

    for (s = selsc_users; s != NULL; s = s->hh.next) {
        printf("[LOG] - user id %d: - [broadcast]\n", s->client.user_id);
        send_packet(s->client.user_fd,0x01,payload);
    }
    pthread_mutex_unlock(&users_mutex);

}

int get_user_fd(int id) {
    pthread_mutex_lock(&users_mutex);
    struct users *user;
    HASH_FIND_INT(selsc_users, &id, user);

    int fd = -1;
    if (user != NULL) {
        fd = user->client.user_fd;
    }

    pthread_mutex_unlock(&users_mutex);
    return fd;
}
