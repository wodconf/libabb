
#ifndef IO_THREAD_H_
#define IO_THREAD_H_
#include <pthread.h>
#include "poller.hpp"
namespace abb {
namespace net{
class IOThread {
public:
	IOThread();
	virtual ~IOThread();
	void Start();
	void StopAndWait();
	Poller& GetPoller(){
		return poller_;
	}
private:
	void Run();
	static void* ThreadMain(void*arg){
		IOThread* t = (IOThread*)arg;
		t->Run();
		return NULL;
	}
	pthread_t pid_;
	Poller poller_;
	bool stop_;
};
}
}


#endif /* IO_THREAD_H_ */
