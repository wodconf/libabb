

#include "context.hpp"
#include "abb/base/log.hpp"
#include "loop.hpp"
namespace abb {
namespace net {
Context::Context()
:threads(NULL),
 loops_(NULL),cur_(0),
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
	cur_ = (cur_+1)%num_loop_;
	return &loops_[cur_];
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
			pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
		}
		if(run_cur_thread_)
			loops_[num_thread_].Start();
	}

}
} /* namespace translate */
} /* namespace adcloud */
