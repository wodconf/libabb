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
enum{
	POLLER_READ = 0x01,
	POLLER_WRITE = 0x02
};
class Poller;
class PollerAble {
public:
	PollerAble(int fd,Poller& p):fd_(fd),event_(0),poller_(p),badd(false){

	}
	virtual ~PollerAble(){

	}
	void Emitter(int revent){
		int ev = revent&event_;
		this->OnEvent(revent&event_);
	}
	int GetEvent(){
		return event_;
	}
	void SetAdd(bool b){
		badd = b;
	}
	bool IsAdd(){
		return badd;
	}
	int GetFd(){
		return fd_;
	}
protected:
	void SetRead(){
		if(event_& POLLER_READ){
			return;
		}
		poller_.SetRead(this);
	}
	void SetWirte(){
		if(event_& POLLER_WRITE){
			return;
		}
		poller_.SetWirte(this);
	}
	void UnSetRead(){
		if(event_& POLLER_READ){
			poller_.UnSetRead(this);
			return;
		}
	}
	void UnSetWrite(){
		if(event_& POLLER_WRITE){
			poller_.UnSetWrite(this);
			return;
		}
	}
private:
	virtual void OnEvent(int event);
private:
	int fd_;
	bool badd;
	Poller& poller_;
	int event_;
};

} /* namespace net */
} /* namespace abb */

#endif /* POLLER_PROXY_HPP_ */
