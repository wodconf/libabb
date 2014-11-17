
#include "abb/net/event_loop.hpp"
#include "abb/net/poller.hpp"
#include "abb/net/singler.hpp"
#include "abb/net/event_io.hpp"
#include "abb/net/timer_set.hpp"
#include "abb/base/log.hpp"
//#include <sys/eventfd.h>
using namespace abb::net;
EventLoop::EventLoop():stop_(false),tid_(pthread_self()){
	//efd_ = eventfd(0, 0);
	poller_ = new Poller();
	sigler_ = new Singler();
	io_event_ = new IOEvent(this,sigler_->GetReadFd(),this);
	timer_set_ = new TimerSet(this);
	io_event_->AllowRead();
}

EventLoop::~EventLoop() {
	io_event_->DisAllowAll();
	delete timer_set_;
	delete poller_;
	delete io_event_;
	delete sigler_;
	//close(efd_);
}
void EventLoop::RunApply(void*arg){
	IOEvent* event = static_cast<IOEvent*>(arg);
	event->GetEventLoop()->poller_->Apply(event);

}
void EventLoop::ApplyIOEvent(IOEvent* event){
	if(this->IsInEventLoop()){
		poller_->Apply(event);
	}else{
		this->QueueInLoop(RunApply,event);
	}
}
int  EventLoop::RunAfter(int ms,EventLoop::timer_fn fn,void*arg){
	if(ms <= 0 || !fn ){ return -1;}
	return timer_set_->AddTimer(ms,false,fn,arg);
}
int  EventLoop::RunEvery(int ms,EventLoop::timer_fn fn,void* arg){
	if(ms <= 0){ return -1;}
	return timer_set_->AddTimer(ms,true,fn,arg);
}
void  EventLoop::Cancel(int id){
	if(id < 0) return ;
	timer_set_->RemoveTimer(id);
}
void EventLoop::Loop(){
	tid_ = pthread_self();
	int min = 1;
	while(!stop_){
		poller_->Poll(min);
		Task task;
		{
			Mutex::Locker lock(mtx_);
			if(!queue_.empty()){
				task = queue_.front();
				queue_.pop();
			}
		}
		if(task.fn){
			task.fn(task.arg);
		}
		min = timer_set_->Process();
		if(min < 0){
			if(min != -1){
				LOG(WARN) << "unknow min "<< min;
			}
			min = 5000;
		}
	}
}
void EventLoop::Stop(){
	stop_ = true;
	sigler_->Write();
}
void EventLoop::RunInLoop(EventLoop::run_fn fn,void*arg){
	if(IsInEventLoop()){
		fn(arg);
	}else{
		QueueInLoop(fn,arg);
	}
}
void EventLoop::QueueInLoop(EventLoop::run_fn fn,void* arg){
	{
		Mutex::Locker lock(mtx_);
		Task task;
		task.fn = fn;
		task.arg = arg;
		queue_.push(task);
	}
	sigler_->Write();
}
void EventLoop::HandleEvent(int event){
	sigler_->Read();
	if(event&IO_EVENT_ERROR){
		LOG(WARN) << "event.loop.sigler.error";
	}
}
