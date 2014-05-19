
#include "abb/net/poller.hpp"
#include "abb/base/log.hpp"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
namespace abb {
namespace net {

Poller::Poller():error_(0) {
	this->efd_ = epoll_create(10);
}

Poller::~Poller() {
	close(efd_);
}
void Poller::AddRead(Entry* arg){
	if(arg->event_&POLLER_READ){
		return;
	}
	if( this->SetEvent(arg->fd_,arg->event_|POLLER_READ,arg,arg->badd_) ){
		arg->event_|=POLLER_READ;
	}
}
void Poller::DelRead(Entry* arg){
	if(arg->event_&POLLER_READ){
		if( this->SetEvent(arg->fd_,arg->event_&(~POLLER_READ),arg,arg->badd_) ){
			arg->event_&=~POLLER_READ;
		}
	}
}
void Poller::AddWrite(Entry* arg){
	if(arg->event_&POLLER_WRITE){
		return;
	}
	if( this->SetEvent(arg->fd_,arg->event_|POLLER_WRITE,arg,arg->badd_) ){
		arg->event_|=POLLER_WRITE;
	}
}
void Poller::DelWrite(Entry* arg){
	if(arg->event_&POLLER_WRITE){
		if( this->SetEvent(arg->fd_,arg->event_&(~POLLER_WRITE),arg,arg->badd_) ){
			arg->event_&=~POLLER_WRITE;
		}
	}
}
void Poller::AddReadWrite(Entry* arg){
	if((arg->event_&POLLER_READ) && (arg->event_&POLLER_WRITE)){
		return;
	}
	if( this->SetEvent(arg->fd_,POLLER_READ|POLLER_WRITE,arg,arg->badd_) ){
		arg->event_=POLLER_READ|POLLER_WRITE;
	}
}
void Poller::DelReadWrite(Entry* arg){
	if((arg->event_&POLLER_READ)|| (arg->event_&POLLER_WRITE)){
		if( this->SetEvent(arg->fd_,0,arg,arg->badd_) ){
			arg->event_ = 0;
		}
	}
}
bool Poller::SetEvent(int fd,int event,Entry* arg,bool bneedadd){
	int mod = bneedadd?EPOLL_CTL_ADD:EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = arg;
	ep_ev.events = 0;
	if(event & POLLER_READ){
		ep_ev.events |=  EPOLLERR | EPOLLIN | EPOLLHUP;
	}
	if(event & POLLER_WRITE){
		ep_ev.events |= EPOLLOUT;
	}
	arg->badd_ = false;
	if(event == 0){
		mod = EPOLL_CTL_DEL;
		arg->badd_ = true;
	}
	int rc = epoll_ctl(efd_,mod,fd,&ep_ev);
	if(rc !=  0){
		error_ = errno;
		LOG(WARN)<< "Poller::SetEvent Fial event:" << event  << "fd:"<<fd<< "code:" << error_ <<"desc:"<< strerror(error_);
	}
	return rc == 0;
}
void Poller::Poll(int timeout){
	struct epoll_event revs[128];
	int rc = epoll_wait(this->efd_,revs,128,timeout);
	if(rc <= 0){
		return;
	}
	//LOG(INFO) << "epoll_wait" << rc;
	int revent;
	Entry * entry;
	for(int i =0;i<rc;i++){
		if(revs[i].events &	(EPOLLIN | EPOLLHUP | EPOLLERR)){
			revent = POLLER_READ;
		}
		if(revs[i].events &	(EPOLLOUT | EPOLLHUP | EPOLLERR)){
			revent |= POLLER_WRITE;
		}
		entry = (Entry*)(revs[i].data.ptr);
		if(entry)
			entry->Emitter(revent);
	}
}

} /* namespace base */
} /* namespace abb */
