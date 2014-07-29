

#include "abb/net/context.hpp"
#include "abb/base/log.hpp"
#include "loop.hpp"
#include <limits.h>
namespace abb {
namespace net {
Context::Context()
:threads(NULL),
 loops_(NULL),
 num_loop_(0),num_thread_(1),
 run_cur_thread_(false),state_(STATE_UNINIT)
{

}
Context::~Context() {
	if(state_ != STATE_UNINIT){
		delete[] threads;
		delete []loops_;
	}

}
Loop* Context::GetFreeLoop(){
	if(state_ == STATE_UNINIT) return NULL;
	int min_use = INT_MAX;
	int cur = 0;
	for(int i=0;i<num_loop_;i++){
		if(loops_[i].GetUse() < min_use){
			min_use = loops_[i].GetUse();
			cur = i;
		}
	}
	return &loops_[cur];
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->Start();
	return NULL;
}
void Context::Init(){
	if(state_ == STATE_UNINIT){
		threads = new pthread_t[num_thread_];
		if(run_cur_thread_){
			loops_ = new Loop[num_thread_+1];
			num_loop_ = num_thread_ + 1;
		}else{
			loops_ = new Loop[num_thread_];
			num_loop_ = num_thread_;
		}
		state_ = STATE_INITED;
	}
}
void Context::Start(){
	if(state_ == STATE_INITED){
		state_ = STATE_STARTED;
		for(int i=0;i<num_thread_;i++){
			pthread_attr_t *thread_attr = NULL;
			thread_attr = (pthread_attr_t*) malloc(sizeof(pthread_attr_t));
			pthread_attr_init(thread_attr);
			pthread_attr_setstacksize(thread_attr, 1024*1024);
			pthread_create(&threads[i],thread_attr,ThreadMain,(void*)(&loops_[i]));

		}
		if(run_cur_thread_)
			loops_[num_thread_].Start();
	}

}
} /* namespace translate */
} /* namespace adcloud */
