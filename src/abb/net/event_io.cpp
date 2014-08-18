
#include "abb/net/event_io.hpp"
#include "abb/net/event_loop.hpp"
namespace abb{
namespace net{
	IOEvent::IOEvent(EventLoop* loop,int fd,IEventHandler* handler)
	:fd_(fd),
	event_(0),
	revent_(0),
	handler_(handler),
	loop_(loop),
	badd_(false){

	}
	IOEvent::IOEvent(EventLoop* loop,IEventHandler* handler)
	:fd_(-1),
	event_(0),
	revent_(0),
	handler_(handler),
	loop_(loop),
	badd_(false)
	{

	}
	IOEvent::~IOEvent(){

	}
	void IOEvent::Apply(){
		this->loop_->ApplyIOEvent(this);
	}
}

}
