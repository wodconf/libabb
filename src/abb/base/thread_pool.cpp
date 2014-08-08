

#include "abb/base/thread_pool.hpp"

#include "abb/base/log.hpp"
namespace abb {

ThreadPool::ThreadPool():bwait_(false),bstop_(false),num_thread_(1),threads(NULL) {
	pthread_mutex_init(&mtx_,NULL);
	pthread_cond_init(&this->cond_,NULL);
}

ThreadPool::~ThreadPool() {
	this->Stop();
	this->Wait();
	pthread_mutex_destroy(&mtx_);
	pthread_cond_destroy(&this->cond_);
	if(this->threads != NULL){
		delete []threads;
	}
}
bool ThreadPool::Start(){
	if(this->threads != NULL){
		return false;
	}
	threads = new pthread_t[num_thread_];
	for(int i=0;i<this->num_thread_;i++){
		pthread_create(&threads[i],NULL,ThreadMain,this);
	}
	return true;
}
void ThreadPool::Wait(){
	if(this->threads == NULL){
		return ;
	}
	if(bwait_)return;
	bwait_ = true;
	for(int i=0;i<this->num_thread_;i++){
		pthread_join(this->threads[i],NULL);
	}
}
void ThreadPool::Stop(){
	if(this->bstop_) return;
	pthread_mutex_lock(&this->mtx_);
	this->bstop_ = true;
	pthread_cond_broadcast(&this->cond_);
	pthread_mutex_unlock(&this->mtx_);
}
void ThreadPool::Execute(CallBack* runer){
	pthread_mutex_lock(&this->mtx_);
	if(this->cb_queue_.empty()){
		this->cb_queue_.push(runer);
		pthread_cond_signal(&this->cond_);
	}else{
		this->cb_queue_.push(runer);
	}

	pthread_mutex_unlock(&this->mtx_);
}
CallBack* ThreadPool::PopCallBack(){
	pthread_mutex_lock(&this->mtx_);
	while( this->cb_queue_.empty() && !bstop_){
		pthread_cond_wait(&this->cond_,&this->mtx_);
	}
	if(bstop_){
		pthread_mutex_unlock(&this->mtx_);
		return NULL;
	}
	CallBack* ptr = this->cb_queue_.front();
	this->cb_queue_.pop();
	pthread_mutex_unlock(&this->mtx_);
	return ptr;
}

void ThreadPool::Worker(){
	while(!bstop_){
		CallBack* ptr = this->PopCallBack();
		if(ptr){
			ptr->Call();
		}
	}
}
void* ThreadPool::ThreadMain(void* arg){
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	pool->Worker();
	return NULL;
} /* namespace common */
} /* namespace adcloud */
