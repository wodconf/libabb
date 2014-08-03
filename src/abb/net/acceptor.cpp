
#include "abb/base/log.hpp"
#include "acceptor.hpp"
#include <unistd.h>
#include "abb/net/socket.hpp"
#include <errno.h>
#include "abb/net/event_loop.hpp"
#include <fcntl.h>
namespace abb {
namespace net {

Acceptor::Acceptor(EventLoop* loop)
:lis_(NULL),
 fd_(-1),
 loop_(loop),
 enable_(false)
{
	io_event_.handler_ = this;
}
Acceptor::~Acceptor() {
	if(fd_ != -1){
		close(fd_);
	}
}
void Acceptor::Destroy(){
	SetEnable(false);
	this->loop_->QueueInLoop(StaticDelete,this);
}
bool Acceptor::Bind(const IPAddr& addr,int* save_err ){
	if( Socket::Listen(&fd_,addr,save_err) ){
		fcntl(fd_, F_SETFD, FD_CLOEXEC);
		addr_ = addr;
		io_event_.fd_ = fd_;
		return true;
	}
	return false;
}
void Acceptor::Close(){
	if(fd_ != -1){
		SetEnable(false);
		this->io_event_.fd_ = -1;
		close(fd_);
		fd_ = -1;
	}
}
void Acceptor::SetEnable(bool benable){
	if(this->enable_ == benable){
		return;
	}
	if(fd_ == -1) return;
	enable_ = benable;
	if(enable_){
		io_event_.SetRead(true);
		this->loop_->ApplyIOEvent(&io_event_);
	}else{
		io_event_.SetRead(false);
		this->loop_->ApplyIOEvent(&io_event_);
	}
}
void Acceptor::HandleEvent(int event){
	if(!enable_)return;
	IPAddr addr;
	int fd;
	if( Socket::Accept(fd_,&fd,&addr,NULL) ){
		addr.family = this->addr_.family;
		fcntl(fd, F_SETFD, FD_CLOEXEC);
		this->lis_->L_Acceptor_OnConnection(this,fd,addr);
	}
}
} /* namespace translate */
} /* namespace adcloud */
