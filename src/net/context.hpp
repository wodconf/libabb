
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>
#include "abb/base/thread_pool.hpp"
#include "io_thread.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context();
	~Context();
	void SetNumPollThread(int num){
		this->num_io_thread = num;
	}
	void SetNumDispatchThread(int num){
		this->pool.SetNumThread(num);
	}
	void Run();
	void WaitAndStop();
	base::ThreadPool& GetThreadPool();
	Poller& GetFreePoller();
private:
	int num_io_thread;
	IOThread* threads;
	bool brund;
	base::ThreadPool pool;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
