
#include "poller.hpp"
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
void Poller::AddRead(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if(arg->event_&POLLER_READ){
		return;
	}
	if( this->SetEvent(arg->fd_,arg->event_|POLLER_READ,arg,arg->badd_) ){
		arg->event_|=POLLER_READ;
	}
}
void Poller::DelRead(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if(arg->event_&POLLER_READ){
		if( this->SetEvent(arg->fd_,arg->event_&(~POLLER_READ),arg,arg->badd_) ){
			arg->event_&=~POLLER_READ;
		}
	}
}
void Poller::AddWrite(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if(arg->event_&POLLER_WRITE){
		return;
	}
	if( this->SetEvent(arg->fd_,arg->event_|POLLER_WRITE,arg,arg->badd_) ){
		arg->event_|=POLLER_WRITE;
	}
}
void Poller::DelWrite(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if(arg->event_&POLLER_WRITE){
		if( this->SetEvent(arg->fd_,arg->event_&(~POLLER_WRITE),arg,arg->badd_) ){
			arg->event_&=~POLLER_WRITE;
		}
	}
}
void Poller::AddReadWrite(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if((arg->event_&POLLER_READ) && (arg->event_&POLLER_WRITE)){
		return;
	}
	if( this->SetEvent(arg->fd_,POLLER_READ|POLLER_WRITE,arg,arg->badd_) ){
		arg->event_=POLLER_READ|POLLER_WRITE;
	}
}
void Poller::DelReadWrite(PollerEntry* arg){
	base::Mutex::Locker l(arg->mtx_);
	if((arg->event_&POLLER_READ)|| (arg->event_&POLLER_WRITE)){
		if( this->SetEvent(arg->fd_,0,arg,arg->badd_) ){
			arg->event_ = 0;
		}
	}
}
bool Poller::SetEvent(int fd,int event,PollerEntry* arg,bool bneedadd){
	int mod = bneedadd?EPOLL_CTL_ADD:EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = arg;
	ep_ev.events = 0;
	if(event & POLLER_READ){
		ep_ev.events |=  EPOLLIN | EPOLLERR  | EPOLLHUP ;
	}
	if(event & POLLER_WRITE){ 
		ep_ev.events |= EPOLLOUT | EPOLLERR | EPOLLHUP ;
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
	int rc = epoll_wait(this->efd_,revs_,num_revs,timeout);
	if(rc <= 0){
		return;
	}
	//LOG(INFO) << "epoll_wait" << rc;
	int revent;
	PollerEntry* entry;
	PollerEntry* last_entry = NULL;
	int last_revent;
	for(int i =0;i<rc;i++){
		if(revs_[i].events &	(EPOLLIN | EPOLLHUP | EPOLLERR)){
			revent = POLLER_READ;
		}
		if(revs_[i].events &	(EPOLLOUT | EPOLLHUP | EPOLLERR)){
			revent |= POLLER_WRITE;
		}
		entry = (PollerEntry*)(revs_[i].data.ptr);
		if(entry){
			if(entry->fd_ == last_emit_fd_){
				last_entry = entry;
				last_revent = revent;
			}else{
				entry->Emitter(revent);
			}
		}	
	}
	if(last_entry)
		last_entry->Emitter(last_revent);
}

} /* namespace base */
} /* namespace abb */
