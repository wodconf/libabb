#ifndef __ABB_NET_IO_EVENT_HPP__
#define __ABB_NET_IO_EVENT_HPP__

#include "abb/net/event_handler.hpp"

namespace abb{
namespace net{
class EventLoop;
class IOEvent{
public:
	IOEvent(EventLoop* loop,int fd,IEventHandler* handler);
	IOEvent(EventLoop* loop,IEventHandler* handler);
	~IOEvent();
	void SetFd(int fd){fd_ = fd;}
	int Fd(){return fd_;}
	int GetEvent(){return event_;};
	void SetRevent(int event){revent_=event;}
	void AllowRead(){event_|= IO_EVENT_READ;Apply();}
	void AllowWrite(){event_|= IO_EVENT_WRITE;Apply();}
	void DisAllowRead(){event_ &= ~IO_EVENT_READ;Apply();}
	void DisAllowWrite(){event_ &= ~IO_EVENT_WRITE;Apply();}
	void DisAllowAll(){event_ = 0;Apply();}
	void AllowAll(){event_ = IO_EVENT_READ| IO_EVENT_WRITE;Apply();}
	void Emitter(){handler_->HandleEvent(revent_);}
	EventLoop* GetEventLoop(){return loop_;}
private:
	void Apply();
	int fd_;
	int event_;
	int revent_;
	IEventHandler* handler_;
	EventLoop* loop_;
public:
	bool badd_;
};
}
}
#endif
