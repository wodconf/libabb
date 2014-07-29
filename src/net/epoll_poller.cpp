
#include "abb/net/epoll_poller.hpp"
#include "abb/net/io_event.hpp"
#include "abb/base/log.hpp"
#include "abb/base/"
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

bool Poller::SetEvent(IOEvent* event){
	int mod = event->flag_ ? EPOLL_CTL_ADD:EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = event;
	ep_ev.events = 0;
	if(event->wait_flag_ & POLLER_READ){
		ep_ev.events |=  EPOLLIN | EPOLLERR  | EPOLLHUP ;
	}
	if(event->wait_flag_ & POLLER_WRITE){ 
		ep_ev.events |= EPOLLOUT | EPOLLERR | EPOLLHUP ;
	}
	if(event->wait_flag_ == 0){
		mod = EPOLL_CTL_DEL;
	}
	int rc = epoll_ctl(efd_,mod,event->fd_,&ep_ev);
	if(rc !=  0){
		int error = errno;
		LOG(WARN)<< "epoll_ctl fail " 
				<< "event: "<<event->wait_flag_  
				<< "fd: "<<event->fd_
				<< "code: " << error 
				<< "desc: "<< strerror(error);
	}
	event->flag_ = event->wait_flag_;
	event->wait_flag_ = 0;
	return rc == 0;
}
void Poller::Poll(int timeout){
	int rc = epoll_wait(this->efd_,revs_,num_revs,timeout);
	if(rc <= 0){
		return;
	}
	IOEvent* event;
	IOEvent* last_event = NULL;
	for(int i =0;i<rc;i++){
		event = (IOEvent*)(revs_[i].data.ptr);
		if(!event) continue;
		if(revs_[i].events &	(EPOLLIN | EPOLLHUP | EPOLLERR)){
			event->pending_flag_ = POLLER_READ;
		}
		if(revs_[i].events &	(EPOLLOUT | EPOLLHUP | EPOLLERR)){
			event->pending_flag_ |= POLLER_WRITE;
		}
		
		if(event->fd_ == last_emit_fd_){
			last_event = event;
		}else{
			event->Emitter();
		}
	}
	if(last_event)
		last_event->Emitter();
}

} /* namespace base */
} /* namespace abb */
