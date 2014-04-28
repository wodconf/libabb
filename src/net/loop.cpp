
#include "loop.hpp"
//#include <sys/eventfd.h>
using namespace abb::net;
Loop::Loop():stop_(false),entry_(this) {
	//efd_ = eventfd(0, 0);
	entry_.SetFd(sigler_->GetReadFd());
	poller_.AddRead(&entry_);
}

Loop::~Loop() {
	poller_.DelRead(&entry_);
	//close(efd_);
}
void Loop::Start(){
	while(!stop_){
		poller_.Poll(1000);
	}
}
void Loop::RunInLoop(run_fn fn,void*arg){
	mtx_.Lock();
	Task task;
	task.fn = fn;
	task.arg = arg;
	queue_.push(task);
	mtx_.UnLock();
	sigler_.Write();
}
void Loop::PollerEvent_OnRead(){
	int i;
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