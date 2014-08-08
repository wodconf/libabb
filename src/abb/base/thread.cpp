#include "abb/base/thread.hpp"
#include "abb/base/signal.hpp"
#include "abb/base/log.hpp"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
namespace abb{
Thread::Thread():tid_(-1),bstart_(false){

}
Thread::~Thread(){
	if(bstart_){
		pthread_detach(tid_);
	}
}
void Thread::Start(Thread::thread_fn fn,void* arg){
	bstart_ = true;
	pthread_attr_t *thread_attr = NULL;
	thread_attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
	pthread_attr_init(thread_attr);
	pthread_attr_setstacksize(thread_attr, 1024*128);
	

	sigset_t old_sigset;
	block_signals(NULL, &old_sigset);
 	pthread_create(&tid_,thread_attr,fn,arg);
  	restore_sigset(&old_sigset);
  	if (thread_attr){
  		free(thread_attr);
  	}
}
void Thread::Wait(){
	if(bstart_){
		pthread_join(tid_,NULL);
		bstart_ = false;
	}
}





int Cond::WaitTimeout(Mutex& mtx,int timeout){
	struct timespec s;
	struct timespec tt;
	clock_gettime(CLOCK_MONOTONIC, &s);
	s.tv_sec +=timeout /1000;
	s.tv_nsec +=(timeout%1000)*1000000;
	s.tv_sec += s.tv_nsec/1000000000;
	s.tv_nsec %= 1000000000;
	int r = pthread_cond_timedwait(&cond_,&mtx.mtx_,&s);
	if(r!= 0 && r != ETIMEDOUT){
		LOG(WARN) << r << strerror(r);
	}
	return r;
}



Notification::Notification():bsingle_(false) {

}
Notification::~Notification() {
}
void Notification::Wait(){
	Mutex::Locker l(mtx_);
	if(!bsingle_){
		cond_.Wait(mtx_);
	}
	bsingle_ =false;
}
bool Notification::WaitTimeout(int timeout){
	Mutex::Locker l(mtx_);
	if(!bsingle_){
		cond_.WaitTimeout(mtx_,timeout);
	}
	bool tmp = bsingle_;
	bsingle_ = false;
	return tmp;
}
void Notification::Notify(){
	Mutex::Locker l(mtx_);
	bsingle_ = true;
	cond_.Broadcast();
}

}
