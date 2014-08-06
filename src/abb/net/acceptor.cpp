
#include "abb/base/log.hpp"
#include "abb/net/acceptor.hpp"
#include "abb/net/socket.hpp"
#include <errno.h>
#include "abb/net/event_loop.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor(EventLoop* loop)
:lis_(NULL),
 enable_(false),
 io_event_(loop,this)
{
	
}
Acceptor::~Acceptor() {
	if(io_event_.Fd() != -1){
		Socket::Close(io_event_.Fd());
	}
}
void Acceptor::Destroy(){
	this->GetEventLoop()->QueueInLoop(StaticDelete,this);
}
bool Acceptor::Bind(const IPAddr& addr,int* save_err ){
	int fd;
	if( Socket::Listen(&fd,addr,save_err) ){
		Socket::SetCloseExec(fd,true,NULL);
		addr_ = addr;
		io_event_.SetFd(fd);
		return true;
	}
	return false;
}
void Acceptor::Close(){
	if(io_event_.Fd() != -1){
		SetEnable(false);
		Socket::Close(io_event_.Fd());
		io_event_.SetFd(-1);
	}
}
void Acceptor::SetEnable(bool benable){
	if(this->enable_ == benable){
		return;
	}
	if(io_event_.Fd() == -1) return;
	enable_ = benable;
	if(enable_){
		io_event_.AllowRead();
	}else{
		io_event_.DisAllowAll();
	}
}
void Acceptor::HandleEvent(int event){
	if(!enable_)return;
	IPAddr addr;
	int fd;
	if( Socket::Accept(io_event_.Fd(),&fd,&addr,NULL) ){
		addr.family = this->addr_.family;
		Socket::SetCloseExec(fd,true,NULL);
		this->lis_->L_Acceptor_OnConnection(this,fd,addr);
	}
}
} /* namespace translate */
} /* namespace adcloud */
