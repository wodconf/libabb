

#include "context.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {
Context* ctx = NULL;
Context::Context():num_io_thread(1),threads(NULL),loops_(NULL),cur_(0),brun(false) {

}
Context::~Context() {
	this->WaitAndStop();
}
Loop& Context::GetFreeLoop(){
	cur_ = (cur_+1)%(this->num_io_thread+1);
	return loops_[cur_];
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->Start();
	return NULL;
}
void Context::Init(){
	if(threads)return;
	threads = new pthread_t[this->num_io_thread];
	loops_ = new Loop[this->num_io_thread+1];
}
void Context::Run(){
	if(!threads || brun)return;
	LOG(INFO)<<"RUN";
	brun = true;
	for(int i=0;i<this->num_io_thread;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	loops_[this->num_io_thread].Start();
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
