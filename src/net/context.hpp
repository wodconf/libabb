
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include <pthread.h>

namespace abb {
namespace net {
class Loop;
class Context {
public:
	Context();
	~Context();
	void SetNumThread(int num){if(state_ == STATE_UNINIT)num_thread_ = num;}
	void SetRunCureentThread(bool run_cur_thread){if(state_ == STATE_UNINIT)run_cur_thread_ = run_cur_thread;}
	void Init();
	void Start();
	Loop* GetFreeLoop();
private:
	pthread_t* threads;
	int num_thread_;
	Loop* loops_;
	bool run_cur_thread_;
	int cur_;
	enum{
		STATE_UNINIT,
		STATE_INITED,
		STATE_STARTED
	}state_;
	int num_loop_;
};
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
