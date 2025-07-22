#ifndef ERPROC_H
#define ERPROC_H

#include <sys/types.h>


int Socket(int domain, int type,int protocol) ;

void Bind(int sockFd,const struct sockaddr *addr, socklen_t addrlen) ;

void Listen(int sockFd, int backlog) ;


int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrlen) ;
void Connect(int sockFd,const struct sockaddr *addr, socklen_t addrlen);


#endif
