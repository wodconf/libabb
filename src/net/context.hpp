
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>
#include "loop.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context();
	~Context();
	void SetNumThread(int num){num_thread_ = num;}
	void SetRunCureentThread(bool run_cur_thread){run_cur_thread_ = run_cur_thread;}
	void Start();
	Loop& GetFreeLoop();
private:
	pthread_t* threads;
	int num_thread_;
	Loop* loops_;
	bool run_cur_thread_;
	int cur_;
	bool brun;
	int num_loop_;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
