#include "abb/net/event_loop_group.hpp"
#include "abb/net/event_loop.hpp"
#include <stdlib.h>
#include <signal.h>

namespace abb{namespace net{
	EventLoopGroup::EventLoopGroup(int threads)
	:num_thread_(threads),cur_(0){
		for(int i=0;i<num_thread_;i++){
			EventLoop* loop = new EventLoop();
			loops_.push_back(loop);
		}
	}
	EventLoopGroup::~EventLoopGroup(){
		for(int i=0;i<loops_.size();i++){
			delete loops_[i];
		}
	}
	void EventLoopGroup::Start(){
		if(threads_.size() > 0) return;
		for(int i=0;i<num_thread_;i++){
			pthread_t tid;
			pthread_attr_t *thread_attr = NULL;
			thread_attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
			pthread_attr_init(thread_attr);
			pthread_attr_setstacksize(thread_attr, 1024*1024);
			pthread_create(&tid,thread_attr,ThreadMain,loops_[i]);
			threads_.push_back(tid);
		}
	}
	void* EventLoopGroup::ThreadMain(void* arg){
		EventLoop* loop = static_cast<EventLoop*>(arg);
		sigset_t sigset;
   		sigfillset(&sigset);
		sigset_t *old_sigset;
		pthread_sigmask(SIG_BLOCK, &sigset, old_sigset);
		loop->Loop();
		return NULL;
	}
	void EventLoopGroup::Stop(){
		for(int i=0;i<loops_.size();i++){
			loops_[i]->Stop();
		}
	}
	void EventLoopGroup::Wait(){
		for(int i=0;i<threads_.size();i++){
			pthread_join(threads_[i],NULL);
		}
	}
	EventLoop* EventLoopGroup::Next(){
		EventLoop* loop = loops_[cur_];
		cur_ = (cur_+1)%loops_.size();
		return loop;
	}
}}
