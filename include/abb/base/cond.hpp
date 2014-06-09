
#ifndef ABB_BASE_COND_HPP_
#define ABB_BASE_COND_HPP_
#include <pthread.h>
#include "mutex.hpp"

namespace abb {
namespace base {

class Cond {
public:
	Cond(){
		pthread_condattr_t attr;
		pthread_condattr_init(&attr);
		pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
		pthread_cond_init(&cond_,&attr);
	}
	~Cond(){
		pthread_cond_destroy(&cond_);
	}
	void Wait(Mutex& mtx){
		pthread_cond_wait(&cond_,&mtx.mtx_);
	}
	void Signal(){
		pthread_cond_signal(&cond_);
	}
	void Broadcast(){
		pthread_cond_broadcast(&cond_);
	}
private:
	pthread_cond_t cond_;
};

} /* namespace common */
} /* namespace adcloud */

#endif /* COND_HPP_ */
