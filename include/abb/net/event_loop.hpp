
#ifndef LOOP_HPP_
#define LOOP_HPP_

#include <queue>
#include <pthread.h>
#include <map>

#include "abb/base/mutex.hpp"
#include "abb/base/define.hpp"
#include "abb/net/event_handler.hpp"


namespace abb {
namespace net{
class Poller;
class Singler;
class IOEvent;
class TimerSet;
typedef int TimeId;
class EventLoop:public IEventHandler {
public:
	typedef void(*run_fn)(void* arg);
	EventLoop();
	virtual ~EventLoop();
	void Loop();
	void Stop();
	void ExecuteInLoop(run_fn fn,void*arg);
	void QueueInLoop(run_fn fn,void* arg);
	TimeId ExecuteAfter(int ms,run_fn fn,void*arg);
	TimeId ExecuteEvery(int ms,run_fn fn,void* arg);
	void Cancel(TimeId id);

	bool IsInEventLoop(){
		return tid_ == pthread_self();
	}
	void ApplyIOEvent(IOEvent* event);
private:
	virtual void HandleEvent(int event);
	static void RunApply(void*arg);
	struct Task{
		Task():fn(NULL),arg(NULL){}
		run_fn fn;
		void* arg;
	};
	std::queue<Task> queue_;
	Poller* poller_;
	IOEvent* io_event_;
	Singler* sigler_;
	TimerSet* timer_set_;
	bool stop_;
	//int efd_;
	base::Mutex mtx_;
	pthread_t tid_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(EventLoop);
};
}
}
#endif /* LOOP_HPP_ */
