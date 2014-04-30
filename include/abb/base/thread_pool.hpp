

#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_

#include <pthread.h>
#include <queue>
namespace abb {
namespace base {

class ThreadPool {
public:
	class Runer{
	public:
		virtual ~Runer(){};
		virtual void Execute() = 0;
	};
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
	void Execute(Runer* runer);
private:
	Runer* PopRuner();
	void Worker();
	static void* ThreadMain(void*);
private:
	bool bwait_;
	bool bstop_;
	std::queue<Runer*> runer_queue_;
	pthread_mutex_t mtx_;
	//pthread_cond_t cond_;
	int num_thread_;
	pthread_t *threads;
};

} /* namespace common */
} /* namespace adcloud */

#endif /* THREAD_POOL_HPP_ */
