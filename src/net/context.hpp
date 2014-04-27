
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>
#include "abb/base/thread_pool.hpp"
#include "loop.hpp"
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
		this->pool.SetNumThread(num);
	}
	void Init();
	void Run();
	void WaitAndStop();
	base::ThreadPool& GetThreadPool(){
		return pool;
	}
	Poller& GetFreePoller();
private:
	int num_io_thread;
	pthread_t* threads;
	Loop* loops_;
	Poller poller;
	base::ThreadPool pool;
	int cur_;
	bool brun;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
