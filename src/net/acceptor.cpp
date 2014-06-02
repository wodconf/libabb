
#include "abb/base/log.hpp"
#include "acceptor.hpp"
#include <unistd.h>
#include "abb/net/socket.hpp"
#include <errno.h>
#include "loop.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor(Loop* loop)
:lis_(NULL),
 fd_(-1),
 loop_(loop),
 entry_(this),
 enable_(false),
 bfreed_(false)
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
	SetEnable(false);
	this->loop_->RunInLoop(StaticDelete,this);
}
bool Acceptor::Listen(const IPAddr& addr,int* save_err ){
	if( Socket::Listen(&fd_,addr,save_err) ){
		addr_ = addr;
		this->entry_.SetFd(fd_);
		return true;
	}
	return false;
}
void Acceptor::SetEnable(bool benable){
	if(this->enable_ == benable){
		return;
	}
	enable_ = benable;
	if(enable_){
		this->loop_->GetPoller().AddRead(&this->entry_);
	}else{
		this->loop_->GetPoller().DelRead(&this->entry_);
	}
}
void Acceptor::PollerEvent_OnRead(){
	if(!enable_)return;
	IPAddr addr;
	int fd;
	if( Socket::Accept(fd_,&fd,&addr,NULL) ){
		this->lis_->L_Acceptor_OnConnection(this,fd,addr);
	}
}
} /* namespace translate */
} /* namespace adcloud */
