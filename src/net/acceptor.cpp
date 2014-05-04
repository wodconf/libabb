
#include "abb/base/log.hpp"
#include "abb/net/acceptor.hpp"
#include "context.hpp"
#include <unistd.h>
#include "socket.hpp"
#include "abb/net/connection.hpp"
#include <errno.h>
#include "loop.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor(Context* ctx)
:lis_(NULL),
 fd_(-1),
 loop_(ctx->GetFreeLoop()),
 entry_(this),
 enable_(false),
 bfreed_(false),
ctx_(ctx)
{

}
Acceptor::~Acceptor() {
	if(fd_ != -1){
		close(fd_);
	}
}
void Acceptor::Destroy(){
	if(bfreed_) return;
	bfreed_ = true;
	enable_ = false;
	this->loop_.GetPoller().DelRead(&this->entry_);
	this->loop_.RunInLoop(StaticDelete,this);
}
bool Acceptor::Bind(const IPAddr& addr){
	fd_ = socket(addr.family,SOCK_STREAM,0);
	if(fd_ < 0){
		int err = errno;
		LOG(INFO)<< "socket" << errno << strerror(errno);
		return false;
	}
	Socket::SetRuseAddr(fd_,true);
	Socket::SetNoBlock(fd_,true);
	if( bind(fd_,&addr.sa.sa,addr.Length()) != 0){
		int err = errno;
		LOG(INFO)<< "bind" << errno << strerror(errno);
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
	if(fd_ < 0){
		return ;
	}
	enable_ = benable;
	LOG(INFO)<< enable_ << fd_;
	if(enable_){
		if( 0 > listen(fd_,10) ){
			int err = errno;
			LOG(INFO)<< "listen:" << errno << strerror(errno);
			return;
		}
		this->loop_.GetPoller().AddRead(&this->entry_);
	}else{
		this->loop_.GetPoller().DelRead(&this->entry_);
	}
}
void Acceptor::PollerEvent_OnRead(){
	if(!enable_)return;
	IPAddr addr;
	socklen_t alen = sizeof(addr.sa);
	int fd = accept(this->fd_, &addr.sa.sa, &alen);
	if(fd < 0){
		return;
	}
	addr.family = addr_.family;
	Connection* conn = Connection::Create(ctx_,fd,addr_,addr);
	this->Dispatch(conn);
}
void Acceptor::DispatchRunner::Execute(){
	LOG(INFO)<<"execute";
	this->ac->lis_->Acceptor_Event(this->ac,this->conn);
	this->ac->UnRef();
	delete this;
}
void Acceptor::Dispatch(Connection* conn){
	this->Ref();
	DispatchRunner* runer = new DispatchRunner(this,conn);
	ctx_->Dispatch(runer);
}
} /* namespace translate */
} /* namespace adcloud */
