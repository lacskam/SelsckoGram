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
        perror("[ERROR] - socket create error - [erproc]");
        printf("\n");
        exit(EXIT_FAILURE);
    }

    return res;
}

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = bind(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - bind error - [erproc]");
        printf("\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

}

void Listen(int sockFd, int backlog) {
    int res = listen(sockFd,backlog);
    if (res == -1) {
        perror("[ERROR] - listen error - [erproc]");
        printf("\n");
        exit(EXIT_FAILURE);
    }


}


int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) {
    int res = accept(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - accept error - [erproc]");
        printf("\n");
        exit(EXIT_FAILURE);
    }



    return res;
}



void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockFd,addr,addrlen);
    if (res == -1) {
        perror("[ERROR] - Connect error - [erproc]");
        printf("\n");
        exit(EXIT_FAILURE);
    }

}

void *Malloc(size_t size) {
    void *ptr=malloc(size);
    if(!ptr)
    {
        perror("[ERROR] - Memory allocation failed - [erproc]");
        printf("\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}


int Write(int fd, void *buf, size_t size)
{
    int _write=write(fd,buf,size);
    if(_write==-1){
        perror("[ERROR] - write error - [erproc]");
        printf("\n");
        close(fd);
        return -1;
    }
    return 0;
}



int Read(int fd, void *buf, size_t size)
{
    ssize_t bytes_read=read(fd,buf,size);
    if(bytes_read==-1) {
        if(errno==ECONNRESET) {
            printf("[ERROR] - Client crashed or connection reset - [erproc]\n");
            return -2; //client crashed
        }
        else if(errno==EBADF) {
            printf("[ERROR] - Bad file descriptor (fd=%d) - [erproc]\n",fd);
            return -2;
        }
        else {
            perror("[ERROR] - Read error: %s - [erproc]\n");
        }
    }
    else if(bytes_read==0) {
        printf("[LOG] - client disconnected - [erporoc]\n");
    }
    return bytes_read;
}
