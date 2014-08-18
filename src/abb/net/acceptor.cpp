
#include "abb/base/log.hpp"
#include "abb/net/acceptor.hpp"
#include "abb/net/socket.hpp"
#include <errno.h>
#include "abb/net/event_loop.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor(EventLoop* loop)
:io_event_(loop,this),
 lis_(NULL),
 enable_(false),
 bclose_(false)
{
	
}
Acceptor::~Acceptor() {
	Close();
}
void Acceptor::Destroy(){
	
	this->GetEventLoop()->QueueInLoop(StaticDelete,this);
}
bool Acceptor::Bind(const IPAddr& addr,int* save_err ){
	int fd;
	if( Socket::Listen(&fd,addr,save_err) ){
		Socket::SetCloseExec(fd,true,NULL);
		Socket::SetNoBlock(fd,true);
		addr_ = addr;
		io_event_.SetFd(fd);
		return true;
	}
	return false;
}
void Acceptor::Close(){
	if(!bclose_){
		SetEnable(false);
		bclose_=true;
		this->GetEventLoop()->RunInLoop((EventLoop::run_fn)Socket::Close,(void*)(io_event_.Fd()));
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
