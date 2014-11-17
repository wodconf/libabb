#include "abb/base/thread.hpp"
#include "abb/base/signal.hpp"
#include "abb/base/log.hpp"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
namespace abb{
Mutex::Locker::Locker(Mutex&mtx):refmtx_(mtx){
	refmtx_.Lock();
}
Mutex::Locker::~Locker(){
	refmtx_.UnLock();
}
Mutex::Mutex(){
	pthread_mutexattr_t attr_;
	pthread_mutexattr_init(&attr_);
	pthread_mutexattr_settype(&attr_,PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&mtx_,&attr_);
	pthread_mutexattr_destroy(&attr_);
};
Mutex::~Mutex(){
	pthread_mutex_destroy(&mtx_);
	
}
void Mutex::Lock(){
	pthread_mutex_lock(&mtx_);
}
void Mutex::UnLock(){
	pthread_mutex_unlock(&mtx_);
}

RWLock::RLocker::RLocker(RWLock&mtx):refmtx_(mtx){
	refmtx_.RLock();
}
RWLock::RLocker::~RLocker(){
	refmtx_.RUnLock();
}

RWLock::WLocker::WLocker(RWLock&mtx):refmtx_(mtx){
	refmtx_.WLock();
}
RWLock::WLocker::~WLocker(){
	refmtx_.WUnLock();
}

RWLock::RWLock(){
	pthread_rwlock_init(&rw_,NULL);
}
RWLock::~RWLock(){
	pthread_rwlock_destroy(&rw_);
}
void RWLock::RLock(){
	pthread_rwlock_rdlock(&rw_);
}
void RWLock::RUnLock(){
	pthread_rwlock_unlock(&rw_);
}
void RWLock::WLock(){
	pthread_rwlock_wrlock(&rw_);
}
void RWLock::WUnLock(){
	pthread_rwlock_unlock(&rw_);
}

Thread::Thread():tid_(-1),bstart_(false){

}
Thread::~Thread(){
	if(bstart_){
		pthread_detach(tid_);
	}
}
void Thread::Start(Thread::thread_fn fn,void* arg,int stacksize){
	bstart_ = true;
	pthread_attr_t thread_attr ;
	pthread_attr_init(&thread_attr);
	pthread_attr_setstacksize(&thread_attr, stacksize);
 	pthread_create(&tid_,&thread_attr,fn,arg);
  	pthread_attr_destroy(&thread_attr);
}
void Thread::Wait(){
	if(bstart_){
		pthread_join(tid_,NULL);
		bstart_ = false;
	}
}
void Thread::Detach(){
	if(bstart_){
		pthread_detach(tid_);
		bstart_ = false;
	}
}

Cond::Cond(){
	pthread_condattr_t attr;
	pthread_condattr_init(&attr);
	pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
	pthread_cond_init(&cond_,&attr);
}
Cond::~Cond(){
	pthread_cond_destroy(&cond_);
}
void Cond::Wait(Mutex& mtx){
	pthread_cond_wait(&cond_,&mtx.mtx_);
}
void Cond::Signal(){
	pthread_cond_signal(&cond_);
}
void Cond::Broadcast(){
	pthread_cond_broadcast(&cond_);
}


int Cond::WaitTimeout(Mutex& mtx,int timeout){
	struct timespec s;
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
