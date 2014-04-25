
#include "poller.hpp"
#include "abb/base/log.hpp"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include "poller_able.hpp"
namespace abb {
namespace net {

Poller::Poller() {
	this->efd_ = epoll_create(10240);
}

Poller::~Poller() {
	close(efd_);
}
void Poller::SetEvent(int fd,int event,PollerAble* arg,bool bneedadd){
	int mod = bneedadd?EPOLL_CTL_ADD:EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = arg;
	ep_ev.events = 0;
	if(event & POLLER_WRITE){
		ep_ev.events |=  EPOLLERR | EPOLLIN | EPOLLHUP;
	}
	if(event & POLLER_READ){
		ep_ev.events |= EPOLLOUT;
	}
	if(ep_ev.events == 0){
		mod = EPOLL_CTL_DEL;
	}
	int rc = epoll_ctl(efd_,mod,fd,&ep_ev);
	if(rc !=  0){
		int err = errno;
		LOG(WARN)<< "Poller::SetEvent Fial event:" << event << << "code:" << err <<"desc:"<< strerror(err);
	}
}
void Poller::Poll(){
	
}

} /* namespace base */
} /* namespace abb */
