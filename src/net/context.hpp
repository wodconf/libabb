
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>
#include "abb/base/thread_pool.hpp"
namespace abb {
namespace net {
class Poller;
class Context {
public:
	Context();
	~Context();
	void Run();
	void WaitAndStop();
	base::ThreadPool& GetThreadPool
private:
	int num_io_thread;
	Poller* pollers;
	pthread_t threads;
	bool brund;
	base::ThreadPool pool;
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
