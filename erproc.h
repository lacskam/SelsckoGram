#ifndef ERPROC_H
#define ERPROC_H

#include <sys/types.h>


int Socket(int domain, int type,int protocol) ;

void *Malloc(size_t size);

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) ;

void Listen(int sockFd, int backlog) ;
int Read(int sockFd,const struct sockaddr *addr, size_t len);
int Write(int sockFd,const struct sockaddr *addr,size_t len);
int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) ;
void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen);


#endif
