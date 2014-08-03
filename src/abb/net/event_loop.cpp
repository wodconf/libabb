
#include "abb/net/event_loop.hpp"
#include "abb/net/poller.hpp"
#include "abb/net/singler.hpp"
#include "abb/net/io_event.hpp"
//#include <sys/eventfd.h>
using namespace abb::net;
EventLoop::EventLoop():stop_(false),tid_(0){
	//efd_ = eventfd(0, 0);
	poller_ = new Poller();
	sigler_ = new Singler();
	io_event_ = new IOEvent(sigler_->GetReadFd(),this);
	io_event_->SetRead(true);
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
	if(event->flag_ | event->wait_flag_ != event->flag_)
		poller_->Apply(event);
}
void EventLoop::Loop(){
	tid_ = pthread_self();
	while(!stop_){
		poller_->Poll(5000);
		Task task;
		{
			base::Mutex::Locker lock(mtx_);
			if(!queue_.empty()){
				task = queue_.front();
				queue_.pop();
			}
		}
		
		if(task.fn){
			task.fn(task.arg);
		}
	}
}
void EventLoop::Stop(){
	stop_ = true;
	sigler_->Write();
}
void EventLoop::RunInLoop(run_fn fn,void*arg){
	if(IsInEventLoop()){
		fn(arg);
	}else{
		QueueInLoop(fn,arg);
	}
}
void EventLoop::RunAfter(run_fn fn,void*arg,int ms){
	
}
void EventLoop::QueueInLoop(run_fn fn,void* arg){
	{
		base::Mutex::Locker lock(mtx_);
		Task task;
		task.fn = fn;
		task.arg = arg;
		queue_.push(task);
	}
	sigler_->Write();
}
void EventLoop::HandleEvent(int event){
	sigler_->Read();
}
