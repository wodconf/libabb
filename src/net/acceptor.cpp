

#include "acceptor.hpp"
#include "context.hpp"
namespace abb {
namespace net {

Acceptor::Acceptor(IEvent* lis):lis_(lis),dispatch_(ctx->GetThreadPool()), {

}
Acceptor::~Acceptor() {

}
bool Acceptor::Bind(const SockAddr& addr){
	this->fd_ = socket(addr.family,SOCK_STREAM,0);
	if(this->fd_ < 0){
		return false;
	}
	Socket::SetRuseAddr(fd_,true);
	Socket::SetNoBlock(fd_,true);
	if( bind(fd_,&addr.sa.sa,addr.Length()) != 0){
		close(fd_);
		return false;
	}
}
void Acceptor::SetEnable(bool benable){
	if(this->enable_ == benable){
		return;
	}
	enable_ = benable;
	if(enable_){

	}
}
void Acceptor::OnEvent(int event){
	SockAddr addr;
	int alen = sizeof(addr.sa);
	int fd = accept(fd_, &addr.sa.sa, &alen);
	if(fd < 0){
		return;
	}
	Connection::New()
}
} /* namespace translate */
} /* namespace adcloud */
