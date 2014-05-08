

#include "context.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {
Context::Context(const ContextOption& option,IProtocolFactory* fac)
:option_(option),
 fac_(fac),
 threads(NULL),
 loops_(NULL),cur_(0),
 brun(false)
{
	threads = new pthread_t[this->option_.GetNumPoller()];
	loops_ = new Loop[this->option_.GetNumPoller()];
}
Context::~Context() {
	delete[] threads;
	delete []loops_;
}
Loop& Context::GetFreeLoop(){
	cur_ = (cur_+1)%(this->option_.GetNumPoller());
	return loops_[cur_];
}
static void* ThreadMain(void* arg){
	Loop* lp = (Loop*)(arg);
	lp->Start();
	return NULL;
}
void Context::Run(bool run_cur_thread){
	if(brun) return;
	brun = true;
	int num = 0;
	if(this->option_.GetRunCurrentThread()){
		num = this->option_.GetNumPoller() -1;
	}else{
		num = this->option_.GetNumPoller();
	}
	for(int i=0;i<num;i++){
		pthread_create(&threads[i],NULL,ThreadMain,(void*)(&loops_[i]));
	}
	if(run_cur_thread)
		loops_[this->option_.GetNumPoller()-1].Start();

}
void Context::Wait(){
	if(brun){
		for(int i=0;i<this->option_.GetNumPoller()-1;i++){
			loops_[i].Stop();
			pthread_join(threads[i],NULL);
		}
		loops_[this->option_.GetNumPoller()-1].Stop();
		if(this->option_.GetRunCurrentThread()){
			pthread_join(threads[this->option_.GetNumPoller()-1],NULL);
		}
	}
}
void Context::Stop(){
	if(brun){
		for(int i=0;i<this->option_.GetNumPoller()-1;i++){
			loops_[i].Stop();
		}
	}
}
} /* namespace translate */
} /* namespace adcloud */
