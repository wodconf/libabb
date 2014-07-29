#include "singler.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include <string.h>
using namespace abb::net;

Singler::Singler(){
	int sockfd[2];
	if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) == -1){
		int err = errno;
		LOG(WARN) << "socketpair" <<err << strerror(err);
	}else{
		rfd_ = sockfd[0];
		wfd_ = sockfd[1];
		Socket::SetNoBlock(rfd_,true);
		Socket::SetNoBlock(wfd_,true);
	}
}
Singler::~Singler(){
	close(rfd_);
	close(wfd_);
}
void Singler::Write(){
	unsigned char  c = 1;
	int nwr;
	Socket::Write(wfd_,&c,sizeof(c),&nwr,NULL);
}
void Singler::Read(){
	unsigned char  c = 1;
	int nrd;
	Socket::Read(rfd_,&c,sizeof(c),&nrd,NULL);
}
