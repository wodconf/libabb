#include "abb/net/event_loop_group.hpp"
#include "abb/net/event_loop.hpp"

namespace abb{namespace net{
	EventLoopGroup::EventLoopGroup(int threads)
	:num_thread_(threads){

	}
	EventLoopGroup::~EventLoopGroup(){
		for(int i=0;i<loops.size();i++){
			delete loops[i];
		}
	}
	void EventLoopGroup::Start(){
		if(threads_.size() > 0) return;
		for(int i=0;i<num_thread_;i++){
			EventLoop* loop = new EventLoop();
			loops.push_back(loop);
			pthread_t tid;
			pthread_attr_t *thread_attr = NULL;
			thread_attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
			pthread_attr_init(thread_attr);
			pthread_attr_setstacksize(thread_attr, 1024*1024);
			pthread_create(&tid,thread_attr,ThreadMain,loop);
			threads_.push_back(tid);
		}
	}
	void EventLoopGroup::ThreadMain(void* arg){
		EventLoop* loop = static_cast<EventLoop*>(arg);
		loop->Loop();
	}
	void EventLoopGroup::Stop(){
		for(int i=0;i<loops.size();i++){
			loops[i]->Stop();
		}
	}
	void EventLoopGroup::Wait(){
		for(int i=0;i<threads_.size();i++){
			pthread_join(threads_[i]);
		}
	}
	EventLoop* Next(){
		EventLoop* loop = loops[cur_];
		cur_ = (cur_+1)%loops.size();
		return loop;
	}
}}