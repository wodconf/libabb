#include "abb/net/connector.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>


namespace abb{namespace net{


Connector::Connector(EventLoop* loop)
:lis_(NULL),
io_event_(loop,this);
connected_(0){
}
Connector::~Connector(){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		Socket::Close(io_event_.Fd());
	}
}
void Connector::Connect(const IPAddr& addr){
	if(__sync_bool_compare_and_swap(&connected_,0,1) ){
		addr_ = addr;
		this->loop_->QueueInLoop(StaticConnect,this);
	}
}
void Connector::RealConnect(){
	int err;
	int fd;
	if( !Socket::Connect(&fd,false,addr_,&err) ){
		this->lis_->L_Connector_OnClose(this,err);
		return;
	}
	Socket::SetCloseExec(fd,true,NULL);
	io_event_.SetFd(fd);
	io_event_.SetWrite(true);	
	this->loop_->ApplyIOEvent(&io_event_);
}
void Connector::RealReset(){
	io_event_.SetWrite(false);
	this->loop_->ApplyIOEvent(&io_event_);
	Socket::Close(io_event_.Fd());
	io_event_.SetFd(-1);
}
void Connector::Destroy(){
	this->loop_->QueueInLoop(StaticDelete,this);
}
void Connector::Reset(){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		this->loop_->QueueInLoop(StaticReset,this);
	}
}
void Connector::HandleEvent(int event){
	if(__sync_bool_compare_and_swap(&connected_,1,0) ){
		io_event_.SetWrite(false);
		this->loop_->ApplyIOEvent(&io_event_);
		int err;
		int fd = io_event_.Fd();
		io_event_.SetFd(-1);
		if( Socket::GetSockError(fd,&err) ){
			if(err == 0){
				this->lis_->L_Connector_OnOpen(this,fd);
				return;
			}else{
				Socket::Close(fd);
			}
		}else{
			err = errno;
			Socket::Close(fd);
		}
		this->lis_->L_Connector_OnClose(this,err);
	}

}

}}
