

#ifndef __ABB_BASE_THREAD_POOL_HPP__
#define __ABB_BASE_THREAD_POOL_HPP__

#include <pthread.h>
#include <queue>
#include "callback.hpp"
#include "define.hpp"
namespace abb {
namespace base {

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();
	void SetNumThread(int num){
		if( num > 0 )
			num_thread_ = num;
	}
	bool Start();
	void Wait();
	void Stop();
	void Execute(CallBack* cb);
private:
	CallBack* PopCallBack();
	void Worker();
	static void* ThreadMain(void*);
private:
	bool bwait_;
	bool bstop_;
	std::queue<CallBack*> cb_queue_;
	pthread_mutex_t mtx_;
	pthread_cond_t cond_;
	int num_thread_;
	pthread_t *threads;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

} 
} 

#endif

