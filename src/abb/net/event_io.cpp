
#include "abb/net/event_io.hpp"
#include "abb/net/event_loop.hpp"
namespace abb{
namespace net{
	IOEvent::IOEvent(EventLoop* loop,int fd,IEventHandler* handler)
	:fd_(fd),
	handler_(handler),
	event_(0),
	loop_(loop),
	badd_(false),
	revent_(0){

	}
	IOEvent::IOEvent(EventLoop* loop,IEventHandler* handler)
	:fd_(-1),
	handler_(handler),
	event_(0),
	loop_(loop),
	badd_(false),
	revent_(0){

	}
	IOEvent::~IOEvent(){

	}
	void IOEvent::Apply(){
		this->loop_->ApplyIOEvent(this);
	}
}

}