#include "abb/net/connector.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>


#include <fcntl.h>

using namespace abb::net;


Connector::Connector(EventLoop* loop)
:fd_(-1),
 lis_(NULL),
 loop_(loop),
 connected_(0){
 	io_event_.handler_ = this;
}
Connector::~Connector(){
	this->Reset();
}
bool Connector::Connect(const IPAddr& addr,int* save_error){
	if(__sync_bool_compare_and_swap(&connected_,0,1) ){
		if( !Socket::Connect(&fd_,false,addr,save_error) ){
			return false;
		}
		fcntl(fd_, F_SETFD, FD_CLOEXEC);
		addr_ = addr;
		io_event_.fd_ = fd_;
		io_event_->SetWrite(true);
		this->loop_->ApplyIOEvent(&io_event_);
		return true;
	}
	return false;
}
void Connector::Destroy(){
	this->loop_->RunInLoop(StaticDelete,this);
}
bool Connector::Reset(){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		io_event_->SetWrite(false);
		this->loop_->ApplyIOEvent(&io_event_);
		Socket::Close(fd_);
		fd_ = -1;
		return true;
	}else{
		return false;
	}
}
void Connector::HandleEvent(int event){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		io_event_->SetWrite(false);
		this->loop_->ApplyIOEvent(&io_event_);
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
		fd_ = -1;
		this->lis_->L_Connector_OnClose(this,err);
	}

}

