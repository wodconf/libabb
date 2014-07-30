
#ifndef LOOP_HPP_
#define LOOP_HPP_

#include "abb/base/mutex.hpp"
#include <queue>
#include <pthread.h>
namespace abb {
namespace net{
class Poller;
class Singler;
class IOEvent;
class EventLoop:public IPollerEvent {
public:
typedef void(*run_fn)(void* arg);
	EventLoop();
	virtual ~EventLoop();
	void Start();
	void Stop();
	void RunInLoop(run_fn fn,void*arg);
	void ApplyIOEvent(IOEvent* event);
	bool IsInEventLoop(){
		return tid_ == pthread_self();
	}
private:
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
	struct Task{
		Task():fn(NULL),arg(NULL){}
		run_fn fn;
		void* arg;
	};
	std::queue<Task> queue_;
	Poller* poller_;
	IOEvent* io_event_;
	Singler* sigler_;
	bool stop_;
	//int efd_;
	base::Mutex mtx_;
	pthread_t tid_;
};
}
}
#endif /* LOOP_HPP_ */
