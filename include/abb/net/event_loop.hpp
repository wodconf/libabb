
#ifndef LOOP_HPP_
#define LOOP_HPP_

#include <queue>
#include <pthread.h>

#include "abb/base/mutex.hpp"
#include "abb/base/define.hpp"
#include "abb/net/event_handler.hpp"


namespace abb {
namespace net{
class Poller;
class Singler;
class IOEvent;
class EventLoop:public IEventHandler {
public:
	typedef void(*run_fn)(void* arg);
	EventLoop();
	virtual ~EventLoop();
	void Loop();
	void Stop();
	void RunInLoop(run_fn fn,void*arg);
	void QueueInLoop(run_fn fn,void* arg);
	void ApplyIOEvent(IOEvent* event);
	bool IsInEventLoop(){
		return tid_ == pthread_self();
	}
	void RunAfter(run_fn fn,void*arg,int ms);
private:
	virtual void HandleEvent(int event);
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
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(EventLoop);
};
}
}
#endif /* LOOP_HPP_ */
