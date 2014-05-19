#include "abb/net/connector.hpp"
#include "context.hpp"
#include "abb/net/connection.hpp"
#include "socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "loop.hpp"
using namespace abb::net;


Connector::Connector(Context* ctx)
:fd_(-1),
 lis_(NULL),
 loop_(ctx->GetFreeLoop()),
 entry_(this),
 bfree(false),ctx_(ctx){

}
Connector::~Connector(){

}
bool Connector::Connect(const IPAddr& addr,int* save_error){
	if(this->fd_ >= 0){
		return false;
	}
	fd_ = socket(addr.family,SOCK_STREAM,0);
	if(fd_ < 0){
		if(save_error) *save_error = errno;
		return false;
	}
	Socket::SetNoBlock(fd_,true);
	if( connect(fd_,&addr.sa.sa,addr.Length()) != 0){
		int err = errno;
		LOG(DEBUG) << err << strerror(err) << fd_;
		if((err == EINPROGRESS) || (err == EAGAIN)){

		}else{
			if(save_error) *save_error = errno;
			close(fd_);
			return false;
		}
	}
	addr_ = addr;
	this->entry_.SetFd(fd_);
	this->loop_.GetPoller().AddWrite(&this->entry_);
	return true;
}
void Connector::Destroy(){
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
	int fd = fd_;
	fd_ = -1;
	this->loop_.GetPoller().DelWrite(&this->entry_);
	this->Ref();
	int err;
	if( Socket::GetSockError(fd,&err) ){
		if(err == 0){
			if(this->lis_){
				Connection* conn= Connection::Create(ctx_,fd,this->addr_,this->addr_);
				this->lis_->L_Connector_EventOpen(conn);
			}else{
				close(fd);
			}
			return;
		}else{
			close(fd);
		}
	}else{
		err = errno;
		close(fd);
	}
	if(this->lis_){
		this->lis_->L_Connector_EventError(err);
	}
	this->UnRef();
}

