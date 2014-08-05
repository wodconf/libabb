#include "abb/base/notify.hpp"
#include "abb/base/log.hpp"

namespace abb{ namespace base {

Notification::Notification() {
	CHECK(pthread_cond_init(&this->cond_,NULL) == 0);
	CHECK(pthread_mutex_init(&this->mtx_,NULL) == 0);
}

Notification::~Notification() {
	pthread_cond_destroy(&this->cond_);
	pthread_mutex_destroy(&this->mtx_);
}
void Notification::Wait(){
	CHECK(pthread_mutex_lock(&this->mtx_) == 0);
	if(!this->notifyed_){
		pthread_cond_wait(&this->cond_,&this->mtx_);
	}
	CHECK(pthread_mutex_unlock(&this->mtx_) == 0);
}
void Notification::Notify(){
	CHECK(pthread_mutex_lock(&this->mtx_)== 0);
	this->notifyed_ = true;
	pthread_cond_broadcast(&this->cond_);
	CHECK(pthread_mutex_unlock(&this->mtx_)== 0);
}
}}