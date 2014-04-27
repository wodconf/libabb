

#include "context.hpp"

namespace abb {
namespace net {

Context::Context():num_io_thread(1),threads(NULL),loops_(NULL) {

}
Context::~Context() {
	this->WaitAndStop();
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->loop();
	return NULL;
}
void Context::Run(){
	if(threads)return;
	threads = new pthread_t[this->num_io_thread];
	loops_ = new Loop[this->num_io_thread];
	for(int i=0;i<this->num_io_thread;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	this->pool.Start();
}
void Context::WaitAndStop(){
	if(threads == NULL) return;
	for(int i=0;i<this->num_io_thread;i++){
		loops_[i].Stop();
		pthread_join(threads[i],NULL);
	}
	delete[] threads;
	delete []loops_;
	threads = NULL;
	this->pool.Stop();
	this->pool.Wait();
}
} /* namespace translate */
} /* namespace adcloud */
