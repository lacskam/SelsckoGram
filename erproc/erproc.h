#ifndef ERPROC_H
#define ERPROC_H

#include <sys/types.h>
#include <sys/socket.h>


int Socket(int domain, int type,int protocol) ;

void *Malloc(size_t size);

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) ;

void Listen(int sockFd, int backlog) ;

int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) ;
void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen);

int Write(int fd, void *buf, size_t size);
int Read(int fd, void *buf, size_t size);

#endif
