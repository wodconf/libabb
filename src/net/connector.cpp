#include "connector.hpp"
#include "context.hpp"
#include "connection.hpp"
#include "socket.hpp"
#include <errno.h>
using namespace abb::net;


Connector::Connector():fd_(-1),lis_(NULL),poller_(ctx->GetFreePoller()){

}
Connector::~Connector(){

}
bool Connector::Connect(const IPAddr& addr){
	int fd_ = socket(addr.family,SOCK_STREAM,0);
	if(fd_ < 0){
		return false;
	}
	Socket::SetRuseAddr(fd_,true);
	Socket::SetNoBlock(fd_,true);
	if( bind(fd_,&addr.sa.sa,addr.Length()) != 0){
		close(fd_);
		return false;
	}
	addr_ = addr;
	this->entry_.SetFd(fd_);
	return true;
}
void Connector::Reset(){
	if(this->fd_){
		this->poller_.DelReadWrite(&this->entry_);
		close(fd_);
		fd_ = -1;
	}
}
void Connector::SetEventCallback(IEvent* ev){lis_=ev;}
void Connector::PollerEvent_OnRead(){}
void Connector::PollerEvent_OnWrite(){
	int err;
	if( Socket::GetSockError(this->fd_,&err) ){
		if(err == 0){
			this->lis_->Connector_Open(Connection::New(fd_,this->addr_,this->addr_));
			return;
		}
	}else{
		err = errno;
	}
	this->lis_->Connector_OpenFail(err);
}
