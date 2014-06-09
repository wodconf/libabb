#include "connector.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "loop.hpp"
#include "abb/net/socket.hpp"
using namespace abb::net;


Connector::Connector(Loop* loop)
:fd_(-1),
 lis_(NULL),
 loop_(loop),
 entry_(this),
 connected_(0){

}
Connector::~Connector(){
	this->Reset();
}
bool Connector::Connect(const IPAddr& addr,int* save_error){
	if(__sync_bool_compare_and_swap(&connected_,0,1) ){
		if( !Socket::Connect(&fd_,false,addr,save_error) ){
			return false;
		}
		addr_ = addr;
		this->entry_.SetFd(fd_);
		this->loop_->GetPoller().AddWrite(&this->entry_);
		return true;
	}
	return false;
}
void Connector::Destroy(){
	this->loop_->RunInLoop(StaticDelete,this);
}
bool Connector::Reset(){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		this->loop_->GetPoller().DelWrite(&this->entry_);
		Socket::Close(fd_);
		fd_ = -1;
		return true;

	}else{
		return false;
	}
}
void Connector::PollerEvent_OnRead(){}
void Connector::PollerEvent_OnWrite(){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		this->loop_->GetPoller().DelWrite(&this->entry_);
		int err;
		if( Socket::GetSockError(fd_,&err) ){
			if(err == 0){
				this->lis_->L_Connector_OnOpen(this,fd_);
				return;
			}else{
				Socket::Close(fd_);
			}
		}else{
			err = errno;
			Socket::Close(fd_);
		}
		this->lis_->L_Connector_OnClose(this,err);
	}

}

