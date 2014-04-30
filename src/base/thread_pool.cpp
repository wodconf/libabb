

#include "abb/base/thread_pool.hpp"

#include "abb/base/log.hpp"
namespace abb {
namespace base {

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
void ThreadPool::Execute(Runer* runer){
	pthread_mutex_lock(&this->mtx_);
	if(this->runer_queue_.empty()){
		this->runer_queue_.push(runer);
		pthread_cond_signal(&this->cond_);
	}else{
		this->runer_queue_.push(runer);
	}

	pthread_mutex_unlock(&this->mtx_);
}
ThreadPool::Runer* ThreadPool::PopRuner(){
	pthread_mutex_lock(&this->mtx_);
	while( this->runer_queue_.empty() && !bstop_){
		pthread_cond_wait(&this->cond_,&this->mtx_);
	}
	if(bstop_){
		pthread_mutex_unlock(&this->mtx_);
		return NULL;
	}
	Runer* ptr = this->runer_queue_.front();
	this->runer_queue_.pop();
	pthread_mutex_unlock(&this->mtx_);
	return ptr;
}

void ThreadPool::Worker(){
	while(!bstop_){
		Runer* ptr = this->PopRuner();
		if(ptr){
			ptr->Execute();
		}
	}
}
void* ThreadPool::ThreadMain(void* arg){
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	pool->Worker();
	return NULL;
}
} /* namespace common */
} /* namespace adcloud */
