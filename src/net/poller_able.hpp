/*
 * poller_proxy.hpp
 *
 *  Created on: 2014Äê4ÔÂ25ÈÕ
 *      Author: goalworld
 */

#ifndef POLLER_PROXY_HPP_
#define POLLER_PROXY_HPP_
#include "poller.hpp"
namespace abb {
namespace net {

class Poller;
class PollerAble {
public:
	PollerAble(int fd,Poller& p)
	:fd_(fd),event_(0),poller_(p),badd_(false){}
	virtual ~PollerAble(){}
	void Emitter(int revent){
		int ev = revent&event_;
		this->OnEvent(revent&event_);
	}
protected:
	void SetRead(){
		if(event_& POLLER_READ){
			return;
		}
		badd_ = true;
		event_|=POLLER_READ;
	}
	void SetWirte(){
		if(event_& POLLER_WRITE){
			return;
		}
		badd_ = true;
		event_|=POLLER_WRITE;
	}
	void UnSetRead(){
		if(event_& POLLER_READ){
			event_ &= ~POLLER_READ;
		}
	}
	void UnSetWrite(){
		if(event_& POLLER_WRITE){
			event_ &= ~POLLER_WRITE;
		}
	}
	void Flush(){
		if(bneedflush_){
			bneedflush_ = false;
			poller_.SetEvent(fd_,event_,this,badd_);
		}
	}
private:
	virtual void OnEvent(int event);
private:
	int fd_;
	bool bneedflush_;
	bool badd_;
	Poller& poller_;
	int event_;
};

} /* namespace net */
} /* namespace abb */

#endif /* POLLER_PROXY_HPP_ */
