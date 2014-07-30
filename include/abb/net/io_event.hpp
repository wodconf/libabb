#ifndef __ABB_NET_IO_EVENT_HPP__
#define __ABB_NET_IO_EVENT_HPP__
#include "abb/net/event_handler.hpp"
namespace abb{
namespace net{

class IOEvent{
public:
	IOEvent()
	:fd_(0),
	handler_(0),
	flag_(0),
	wait_flag_(0),
	pending_flag_(0){

	}
	IOEvent(int fd,IEventHandler* handler)
	:fd_(fd),
	handler_(handler),
	flag_(0),
	wait_flag_(0),
	pending_flag_(0){

	}
	~IOEvent(){};
	int fd_;
	int flag_;
	int wait_flag_;
	int pending_flag_;
	IEventHandler* handler_;
public:
	void SetRead(bool b){
		if(b){
			wait_flag_ |= IO_EVENT_READ;
		}else{
			wait_flag_ &= ~IO_EVENT_READ;	
		}
	}
	void SetWrite(bool b){
		if(b){
			wait_flag_ |= IO_EVENT_WRITE;
		}else{
			wait_flag_ &= ~IO_EVENT_WRITE;	
		}
	}
	void Emitter(){
		handler_->HandleEvent(flag_&pending_flag_);
	}
};
}
}
#endif
