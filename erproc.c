#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "erproc.h"

int Socket(int domain, int type,int protocol) {
    int res = socket(domain, type, protocol);
    if (res == -1) {
        perror("socket create error\n");
        exit(EXIT_FAILURE);
    }

    return res;
}

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = bind(sockFd,addr,addrlen);
    if (res == -1) {
        perror("bind error\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

}

void Listen(int sockFd, int backlog) {
    int res = listen(sockFd,backlog);
    if (res == -1) {
        perror("listen error\n");
        exit(EXIT_FAILURE);
    }


}


int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) {
    int res = accept(sockFd,addr,addrlen);
    if (res == -1) {
        perror("accept error\n");
        exit(EXIT_FAILURE);
    }



    return res;
}



void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockFd,addr,addrlen);
    if (res == -1) {
        perror("connect error\n");
        exit(EXIT_FAILURE);
    }

}
