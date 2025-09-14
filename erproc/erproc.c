#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "erproc.h"

int Socket(int domain, int type,int protocol) {
    int res = socket(domain, type, protocol);
    if (res == -1) {
        perror("[ERROR] - socket create error - [erproc]\n");
        exit(EXIT_FAILURE);
    }

    return res;
}

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = bind(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - bind error - [erproc]\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

}

void Listen(int sockFd, int backlog) {
    int res = listen(sockFd,backlog);
    if (res == -1) {
        perror("[ERROR] - listen error - [erproc]\n");
        exit(EXIT_FAILURE);
    }


}


int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) {
    int res = accept(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - accept error - [erproc]\n");
        exit(EXIT_FAILURE);
    }



    return res;
}



void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - connect error - [erproc]\n");
        exit(EXIT_FAILURE);
    }

}

void *Malloc(size_t size) {
    void *ptr=malloc(size);
    if(!ptr)
    {
        perror("[ERROR] - memory allocation failed - [erproc]\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}


int Write(int fd, void *buf, size_t size)
{
    int _write=write(fd,buf,size);
    if(_write==-1){
        perror("[ERROR] - write error - [erproc]\n");
        close(fd);
        return -1;
    }
    return 0;
}



int Read(int fd, void *buf, size_t size)
{
    ssize_t bytes_read=read(fd,buf,size);
    if(bytes_read==-1){
        if(errno==ECONNRESET){
            perror("[ERROR] - Client crashed or connection reset - [erproc]\n");
            return -2; //client crashed
        }
        else{
            perror("[ERROR] - read error: %s - [erproc]\n");
        }
    }
    else if(bytes_read==0){
        printf("[LOG] - client disconnected - [erporoc]\n");
    }
    return bytes_read;
}
