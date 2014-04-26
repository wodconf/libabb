

#include "acceptor.hpp"
#include "context.hpp"
#include <unistd.h>
#include "socket.hpp"
#include "connection.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor()
:lis_(NULL),
fd_(-1),
dispatch_(ctx->GetThreadPool()),
poller_(ctx->GetFreePoller()),
entry_(this),
enable_(false)
{

}
Acceptor::~Acceptor() {
	if(fd_ != -1){
		close(fd_);
	}
}
bool Acceptor::Bind(const IPAddr& addr){
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
void Acceptor::SetEnable(bool benable){
	if(this->enable_ == benable){
		return;
	}
	enable_ = benable;
	if(enable_){
		listen(fd_,10);
		this->poller_.AddRead(&this->entry_);
	}else{
		this->poller_.DelRead(&this->entry_);
	}
}
void Acceptor::PollerEvent_OnRead(){
	IPAddr addr;
	socklen_t alen = sizeof(addr.sa);
	int fd = accept(this->fd_, &addr.sa.sa, &alen);
	if(fd < 0){
		return;
	}
	Connection* conn = Connection::New(fd,addr_,addr);
	this->Dispatch(conn);
}
void Acceptor::DispatchRunner::Execute(){
	this->ac->lis_->Acceptor_Event(this->ac,this->conn);
	this->ac->UnRef();
	delete this;
}
void Acceptor::Dispatch(Connection* conn){
	this->Ref();
	DispatchRunner* runer = new DispatchRunner();
	runer->conn = conn;
	this->dispatch_.Execute(runer);
}
} /* namespace translate */
} /* namespace adcloud */
