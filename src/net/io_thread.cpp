
#include "io_thread.hpp"

using namespace abb::net;

IOThread::IOThread():pid_(-1),stop_(false) {

}

IOThread::~IOThread() {
	if(!stop_){
		this->StopAndWait();
	}
}

void IOThread::Start(){
	if(pid_ == -1){
		pthread_create(&pid_,NULL,ThreadMain,this);
	}
}
void IOThread::StopAndWait(){
	stop_ = true;
	if(pid_ == -1){
		pthread_join(this->pid_,NULL);
		return;
	}
}
void IOThread::Run(){
	while(!stop_){
		poller_.Poll(1000);
	}
}
