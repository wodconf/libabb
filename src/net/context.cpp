

#include "context.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {
Context::Context()
:option_(option),
 threads(NULL),
 loops_(NULL),cur_(0),
 brun(false)
{
	
}
Context::~Context() {
	if(brun){
		delete[] threads;
		delete []loops_;
	}
	
}
Loop& Context::GetFreeLoop(){
	cur_ = (cur_+1)%num_loop_;
	return loops_[cur_];
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->Start();
	return NULL;
}
void Context::Start(){
	if(brun) return;
	brun = true;
	threads = new pthread_t[num_thread_];
	if(run_cur_thread_){
		loops_ = new Loop[num_thread_+1];
		num_loop_ = num_thread_ + 1;
	}else{
		loops_ = new Loop[num_thread_];
		num_loop_ = num_thread_;
	}
	for(int i=0;i<num_thread_;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	if(run_cur_thread_)
		loops_[num_thread_].Start();

}
} /* namespace translate */
} /* namespace adcloud */
