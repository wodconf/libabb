#include "abb/net/connector.hpp"
#include "context.hpp"
#include "abb/net/connection.hpp"
#include "socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "loop.hpp"
using namespace abb::net;


Connector::Connector(Loop* loop)
:fd_(-1),
 lis_(NULL),
 loop_(loop),
 entry_(this),
 bfree(false){

}
Connector::~Connector(){

}
bool Connector::Connect(const IPAddr& addr,int* save_error){
	if(this->fd_ >= 0){
		return false;
	}
	if( !Socket::Connect(&fd_,false,addr,save_error) ){ 
		return false;
	}
	addr_ = addr;
	this->entry_.SetFd(fd_);
	this->loop_->GetPoller().AddWrite(&this->entry_);
	return true;
}
void Connector::Destroy(){
	if(bfree)return;
	bfree = true;
	this->Reset();
	this->loop_->RunInLoop(StaticDelete,this);
}
void Connector::Reset(){
	if(this->fd_){
		this->loop_->GetPoller().DelWrite(&this->entry_);
		close(fd_);
		fd_ = -1;
	}
}
void Connector::PollerEvent_OnRead(){}
void Connector::PollerEvent_OnWrite(){
	if( fd_ == -1) return;
	int fd = fd_;
	fd_ = -1;
	this->loop_->GetPoller().DelWrite(&this->entry_);
	int err;
	if( Socket::GetSockError(fd,&err) ){
		if(err == 0){
			this->lis_->L_Connector_OnOpen(this,fd);
			return;
		}else{
			close(fd);
		}
	}else{
		err = errno;
		close(fd);
	}
	this->lis_->L_Connector_OnClose(err);
}

