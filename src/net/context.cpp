

#include "context.hpp"

namespace abb {
namespace net {
extern Context* ctx = NULL;
Context::Context():num_io_thread(1),threads(NULL),loops_(NULL),cur_(0) {

}
Context::~Context() {
	this->WaitAndStop();
}
Poller& Context::GetFreePoller(){
	cur_ = (cur_+1)%(this->num_io_thread+1);
	return loops_[cur_].GetPoller();
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->loop();
	return NULL;
}
void Context::Run(){
	if(threads)return;
	threads = new pthread_t[this->num_io_thread];
	loops_ = new Loop[this->num_io_thread+1];
	for(int i=0;i<this->num_io_thread;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	loops_[this->num_io_thread].loop();
	this->pool.Start();
}
void Context::WaitAndStop(){
	if(threads == NULL) return;
	for(int i=0;i<this->num_io_thread;i++){
		loops_[i].Stop();
		pthread_join(threads[i],NULL);
	}
	loops_[this->num_io_thread].Stop();
	delete[] threads;
	delete []loops_;
	threads = NULL;
	this->pool.Stop();
	this->pool.Wait();
}
} /* namespace translate */
} /* namespace adcloud */
