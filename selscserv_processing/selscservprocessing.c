#include "selscservprocessing.h"

pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;


void* handle_client(void* arg)
{
    int fd = *(int*)arg;
    free(arg);
    int id=push_connected_client(fd);
    int* ping_id=Malloc(sizeof(int));
    *ping_id =id;

    pthread_t ping_tid;

    if(pthread_create(&ping_tid,NULL,ping_client,ping_id)!=0) {
        perror("[ERROR] - Ping thread error - [handle_client]");
    }
    else {
        pthread_detach(ping_tid);
    }


    while(1) {
        uint8_t *payload = NULL;
        int nread = parce_packet(fd, &payload);

        if(nread==-2) {   //client crashed or disconnected
            free(payload);
            break;
        }
        if (!nread) {
            perror("[ERROR] - Error to parce packet - [handle_client]");
            continue;
        }


        // send_packet(fd,0x01,payload);
        broadcast(payload);

        free(payload);

    }
    remove_disconnected_client(id);
    return NULL;

}


int push_connected_client(int fd)
{
    if (fd == -1) {
        perror("[ERROR] - Invalid file descriptor - [push_connected_client]\n");
        return -1;
    }

    static int id_count = 0;
    int id = id_count++;

    pthread_mutex_lock(&users_mutex);
    struct users *user;

    HASH_FIND_INT(selsc_users, &id, user);
    if (!user) {
        user = (struct users *)Malloc(sizeof *user);
        user->id=id;
        user->client.user_fd = fd;
        user->client.user_id = id;
        HASH_ADD_INT(selsc_users,id,user);
        printf("[LOG] - Client added (id:%d)- [push_connected client]\n",user->client.user_id);
    }
    else {
        puts("[ERROR] - User has already been created - [push_connected client]\n");
        pthread_mutex_unlock(&users_mutex);
        return -2;
    }
    pthread_mutex_unlock(&users_mutex);
    return id;
}


void* ping_client(void* arg)
{
    int id=*(int*)arg;
    free(arg);
    struct users *user;
    pthread_mutex_lock(&users_mutex);
    HASH_FIND_INT(selsc_users,&id,user);

    if(!user) {
        pthread_mutex_unlock(&users_mutex);
        printf("[ERROR] - Client was not found (id:%d) - [ping_client]\n",id);
        return NULL;
    }
    int fd=user->client.user_fd;
    pthread_mutex_unlock(&users_mutex);
    int max_attempts=3,failed_attempts=0;

    while(max_attempts>failed_attempts) {
        sleep(5);

        pthread_mutex_lock(&users_mutex);
        HASH_FIND_INT(selsc_users,&id,user);
        if(!user) {
            pthread_mutex_unlock(&users_mutex);
            break;
        }
        pthread_mutex_unlock(&users_mutex);
        if(send_packet(fd,PACKET_TYPE_PING,"ping")!=0) {
            failed_attempts++;
            printf("[WARN] - Client does not respond (id:%d) - [ping_client]\n",id);
        }
        else {
            failed_attempts=0;
        }
    }

    return NULL;
}

int remove_disconnected_client(int id)
{
    struct users *user;
    pthread_mutex_lock(&users_mutex);
    HASH_FIND_INT(selsc_users,&id,user);
    if(user) {
        HASH_DEL(selsc_users,user);
        close(user->client.user_fd);
        free(user);
        pthread_mutex_unlock(&users_mutex);
        printf("[LOG] - Client (id: %d) removed - [remove_disconnected_client]\n", id);
        return 0;
    }
    pthread_mutex_unlock(&users_mutex);
    puts("[ERROR] - Client was not found - [remove_disconnected_client]\n");
    return -1;
}




void broadcast(uint8_t *payload) {
    struct users *s;
    pthread_mutex_lock(&users_mutex);
    for (s = selsc_users; s != NULL; s = s->hh.next) {
        printf("[LOG] - User id %d: - [broadcast]\n", s->client.user_id);
        send_packet(s->client.user_fd,PACKET_TYPE_MESSAGE,payload);
    }
    pthread_mutex_unlock(&users_mutex);
}
