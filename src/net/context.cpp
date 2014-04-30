

#include "context.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {
Context* ctx = NULL;
Context::Context():num_io_thread(1),threads(NULL),loops_(NULL),cur_(0),brun(false),pool(NULL),num_dis_thread(0) {

}
Context::~Context() {
	this->WaitAndStop();
}
Loop& Context::GetFreeLoop(){
	cur_ = (cur_+1)%(this->num_io_thread);
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
	loops_ = new Loop[this->num_io_thread];
	if(this->num_dis_thread > 0){
		this->pool = new base::ThreadPool();
		this->pool->SetNumThread(this->num_dis_thread);
	}
}
void Context::Run(bool run_cur_thread){
	if(!threads || brun)return;
	LOG(INFO)<<"RUN";
	brun = true;
	int num = 0;
	if(run_cur_thread){
		num = this->num_io_thread -1;
	}else{
		num = this->num_io_thread;
	}
	for(int i=0;i<num;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	if(pool)
		this->pool->Start();
	if(run_cur_thread)
		loops_[this->num_io_thread-1].Start();

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
	if(this->pool){
		this->pool->Stop();
		this->pool->Wait();
		delete this->pool;
		this->pool = NULL;
	}
}
} /* namespace translate */
} /* namespace adcloud */
