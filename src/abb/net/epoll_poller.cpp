
#include "abb/net/epoll_poller.hpp"
#include "abb/net/io_event.hpp"
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
	int mod = event->flag_ ? EPOLL_CTL_MOD:EPOLL_CTL_ADD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = event;
	ep_ev.events = 0;
	if(event->wait_flag_ & IO_EVENT_READ){
		ep_ev.events |=  EPOLLIN | EPOLLERR  | EPOLLHUP ;
	}
	if(event->wait_flag_ & IO_EVENT_WRITE){
		ep_ev.events |= EPOLLOUT | EPOLLERR | EPOLLHUP ;
	}
	if(event->wait_flag_ == 0){
		mod = EPOLL_CTL_DEL;
	}
	int rc = epoll_ctl(efd_,mod,event->fd_,&ep_ev);
	if(rc !=  0){
		int error = errno;
		LOG(WARN)<< "epoll_ctl fail " 
				<< "flag: "<<event->flag_  
				<< "wait_flag: "<<event->wait_flag_  
				<< "fd: "<<event->fd_
				<< "code: " << error 
				<< "desc: "<< strerror(error);
		if(errno == EEXIST){
			epoll_ctl(efd_,EPOLL_CTL_MOD,event->fd_,&ep_ev);
		}
	}
	event->flag_ = event->wait_flag_;
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
		if(revs_[i].events &	(EPOLLIN | EPOLLHUP | EPOLLERR)){
			event->pending_flag_ = IO_EVENT_READ;
		}
		if(revs_[i].events &	(EPOLLOUT | EPOLLHUP | EPOLLERR)){
			event->pending_flag_ |= IO_EVENT_WRITE;
		}
		event->Emitter();
	}
}

} /* namespace base */
} /* namespace abb */
