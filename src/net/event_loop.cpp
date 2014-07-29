
#include "abb/net/event_loop.hpp"
#include "abb/net/poller.hpp"
#include "abb/net/singler.hpp"
#include "abb/net/io_event.hpp"
//#include <sys/eventfd.h>
using namespace abb::net;
EventLoop::EventLoop():stop_(false){
	//efd_ = eventfd(0, 0);
	poller_ = new Poller();
	sigler_ = new Singler();
	io_event_ = new IOEvent(sigler_->GetReadFd(),this)
	io_event_->SetRead(true);

	poller_->SetLastEmitFd(sigler_->GetReadFd());
	poller_->Apply(io_event_);
}

EventLoop::~EventLoop() {
	io_event_->SetRead(false);
	poller_->Apply(io_event_);
	delete poller_;
	delete io_event_;
	delete sigler_;
	//close(efd_);
}
void EventLoop::ApplyIOEvent(IOEvent* event){
	poller_->Apply(event);
}
void EventLoop::Loop(){
	tid_ = pthread_self();
	while(!stop_){
		poller_->Poll(5000);
	}
}
void EventLoop::Stop(){
	stop_ = true;
	sigler_.Write();
}
void EventLoop::RunInLoop(run_fn fn,void*arg){
	mtx_.Lock();
	Task task;
	task.fn = fn;
	task.arg = arg;
	queue_.push(task);
	mtx_.UnLock();
	sigler_.Write();
}
void EventLoop::PollerEvent_OnRead(){
	sigler_.Read();
	mtx_.Lock();
	if(!queue_.empty()){
		Task task = queue_.front();
		queue_.pop();
		mtx_.UnLock();
		task.fn(task.arg);
	}else{
		mtx_.UnLock();
	}
}
