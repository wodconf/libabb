
#ifndef LOOP_HPP_
#define LOOP_HPP_
#include "abb/net/poller.hpp"
#include "abb/net/i_poller_event.hpp"
#include "abb/base/mutex.hpp"
#include <queue>
#include "singler.hpp"
namespace abb {
namespace net{
class Loop:public IPollerEvent {
public:
typedef void(*run_fn)(void* arg);
	Loop();
	virtual ~Loop();
	void Start();
	void Stop(){
		stop_ = true;
	}
	int SetTimeout();
	void ClearTimeout();
	void RunInLoop(run_fn fn,void*arg);
	Poller& GetPoller(){
		return poller_;
	}
private:
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
	struct Task{
		run_fn fn;
		void* arg;
	};
	std::queue<Task> queue_;
	Poller poller_;
	Poller::Entry entry_;
	bool stop_;
	//int efd_;
	base::Mutex mtx_;
	Singler sigler_;
};
}
}
#endif /* LOOP_HPP_ */
