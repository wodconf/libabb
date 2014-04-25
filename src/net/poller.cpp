/*
 * poller.cpp
 *
 *  Created on: 2014Äê4ÔÂ25ÈÕ
 *      Author: goalworld
 */

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

void Poller::SetRead(PollerAble* proxy){
	int mod = EPOLL_CTL_MOD;
	if(proxy->IsAdd()){
		mod = EPOLL_CTL_ADD;
		proxy->SetAdd(true);
	}
	struct epoll_event ep_ev;
	ep_ev.data.ptr = proxy;
	ep_ev.events =  EPOLLERR | EPOLLIN | EPOLLHUP;
	if(proxy->GetEvent() & POLLER_WRITE){
		ep_ev.events |= EPOLLOUT;
	}
	int rc = epoll_ctl(efd_,mod,proxy->GetFd(),&ep_ev);
	if(rc !=  0){
		int err = errno;
		LOG(WARN)<< "Poller::SetRead Fial" << "code:" << err << strerror(err);
	}
}
void Poller::UnSetRead(PollerAble* proxy){
	int mod = EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = proxy;
	ep_ev.events  = 0;
	if(proxy->GetEvent() & POLLER_WRITE){
		ep_ev.events |= EPOLLOUT;
	}else{
		mod = EPOLL_CTL_DEL;
		proxy->SetAdd(false);
	}
	int rc = epoll_ctl(efd_,mod,proxy->GetFd(),&ep_ev);
	if(rc !=  0){
		int err = errno;
		LOG(WARN)<< "Poller::UnSetRead Fial" << "code:" << err << strerror(err);
	}
}
void Poller::SetWrite(PollerAble* proxy){
	int mod = EPOLL_CTL_MOD;
	if(proxy->IsAdd()){
		mod = EPOLL_CTL_ADD;
		proxy->SetAdd(true);
	}
	struct epoll_event ep_ev;
	ep_ev.data.ptr = proxy;
	ep_ev.events  = EPOLLOUT;
	if(proxy->GetEvent() & POLLER_READ){
		ep_ev.events |= EPOLLIN;
	}
	int rc = epoll_ctl(efd_,mod,proxy->GetFd(),&ep_ev);
	if(rc !=  0){
		int err = errno;
		LOG(WARN)<< "Poller::SetWrite Fial" << "code:" << err << strerror(err);
	}
}
void Poller::UnSetWrite(PollerAble* proxy){
	int mod = EPOLL_CTL_MOD;
	struct epoll_event ep_ev;
	ep_ev.data.ptr = proxy;
	ep_ev.events  = 0;
	if(proxy->GetEvent() & POLLER_READ){
		ep_ev.events |= EPOLLIN;
	}else{
		mod = EPOLL_CTL_DEL;
		proxy->SetAdd(false);
	}
	int rc = epoll_ctl(efd_,mod,proxy->GetFd(),&ep_ev);
	if(rc !=  0){
		int err = errno;
		LOG(WARN)<< "Poller::UnSetWrite Fial" << "code:" << err << strerror(err);
	}
}

} /* namespace base */
} /* namespace abb */
