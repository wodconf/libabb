
#include "abb/net/epoll_poller.hpp"
#include "abb/net/event_io.hpp"
#include "abb/base/log.hpp"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
namespace abb {
namespace net {

Poller::Poller():error_(0) {
	this->efd_ = epoll_create(10);
	fcntl(efd_, F_SETFD, FD_CLOEXEC);

}

Poller::~Poller() {
	close(efd_);
}

void Poller::Apply(IOEvent* event){
	int mod = event->badd_ ? EPOLL_CTL_MOD:EPOLL_CTL_ADD;
	event->badd_ = true;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = event;
	ep_ev.events = 0;
	if(event->GetEvent() & IO_EVENT_READ){
		ep_ev.events |=  EPOLLIN | EPOLLERR  | EPOLLHUP ;
	}
	if(event->GetEvent() & IO_EVENT_WRITE){
		ep_ev.events |= EPOLLOUT | EPOLLERR | EPOLLHUP ;
	}
	if(event->GetEvent() == 0){
		mod = EPOLL_CTL_DEL;
		event->badd_ = false;
	}
	int rc = epoll_ctl(efd_,mod,event->Fd(),&ep_ev);
	if(rc !=  0){
		int error = errno;
		LOG(WARN)<< "epoll_ctl fail " 
				<< "flag: "<<event->GetEvent()
				<< "fd: "<<event->Fd()
				<< "code: " << error 
				<< "desc: "<< strerror(error);
		if(errno == EEXIST){
			epoll_ctl(efd_,EPOLL_CTL_MOD,event->Fd(),&ep_ev);
		}
	}
	return ;
}
void Poller::Poll(int timeout){
	int rc = epoll_wait(this->efd_,revs_,num_revs,timeout);
	if(rc <= 0){
		return;
	}
	IOEvent* event;
	for(int i =0;i<rc;i++){
		event = (IOEvent*)(revs_[i].data.ptr);
		if(!event) continue;
		int revent = 0;
		if(revs_[i].events &	(EPOLLIN | EPOLLHUP | EPOLLERR)){
			revent = IO_EVENT_READ;
		}
		if(revs_[i].events &	(EPOLLOUT | EPOLLHUP | EPOLLERR)){
			revent|= IO_EVENT_WRITE;
		}
		event->SetRevent(revent);
		event->Emitter();
	}
}

} /* namespace base */
} /* namespace abb */
