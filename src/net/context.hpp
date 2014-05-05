
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>
#include "abb/base/thread_pool.hpp"
#include "loop.hpp"
#include "callback.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context();
	~Context();
	void SetNumPollThread(int num){
		if(threads == NULL)
			this->num_io_thread = num;
	}
	void SetNumDispatchThread(int num){
		if(pool == NULL)
			num_dis_thread = num;
	}
	void Init();
	void Run(bool run_cur_thread);
	void WaitAndStop();
	void Dispatch(base::CallBack* cb){
		if(pool){
			this->pool->Execute(cb);
		}else{
			cb->Call();
		}
	}
	Loop& GetFreeLoop();
private:
	int num_dis_thread;
	int num_io_thread;
	pthread_t* threads;
	Loop* loops_;
	base::ThreadPool* pool;
	int cur_;
	bool brun;
	bool run_cur_thread_;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
