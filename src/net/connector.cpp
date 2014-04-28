#include "abb/net/connector.hpp"
#include "context.hpp"
#include "abb/net/connection.hpp"
#include "socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
using namespace abb::net;


Connector::Connector():fd_(-1),lis_(NULL),loop_(ctx->GetFreeLoop()),entry_(this),bfree(false){

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
	if( connect(fd_,&addr.sa.sa,addr.Length()) != 0){
		int err = errno;
		if((err == EINPROGRESS) || (err == EAGAIN)){
			return 0;
		}else{
			LOG(INFO)<< "connect:" << errno << strerror(errno);
			return -1;
		}
	}
	addr_ = addr;
	this->entry_.SetFd(fd_);
	this->loop_.GetPoller().AddWrite(&this->entry_);
	return true;
}
void Connector::Delete(){
	if(bfree)return;
	bfree = true;
	this->Reset();
	this->loop_.RunInLoop(StaticDelete,this);
}
void Connector::Reset(){
	if(this->fd_){
		this->loop_.GetPoller().DelWrite(&this->entry_);
		close(fd_);
		fd_ = -1;
	}
}
void Connector::PollerEvent_OnRead(){}
void Connector::PollerEvent_OnWrite(){
	if( fd_ == -1) return;
	this->loop_.GetPoller().DelWrite(&this->entry_);
	DispatchRunner* r = new DispatchRunner();
	int err;
	if( Socket::GetSockError(this->fd_,&err) ){
		if(err == 0){
			r->conn = Connection::New(fd_,this->addr_,this->addr_);
		}
	}else{
		err = errno;
	}
	r->err = err;
	this->dispatch_.Execute(r);
}
void Connector::DispatchRunner::Execute(){
	if(conn){
		self->lis_->Connector_Open(Connection::New(fd_,self->addr_,self->addr_));
	}else{
		this->lis_->Connector_OpenFail(err);
	}
	delete this;
}