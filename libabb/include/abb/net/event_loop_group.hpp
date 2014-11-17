#ifndef __ABB_NET_EVENT_LOOP_GROUP_HPP__
#define __ABB_NET_EVENT_LOOP_GROUP_HPP__

#include <vector>
#include <pthread.h>

#include "abb/base/define.hpp"

namespace abb{namespace net{
class EventLoop;
class EventLoopGroup{
public:
	explicit EventLoopGroup(int threads);
	~EventLoopGroup();
	void Start();
	void Stop();
	void Wait();
	EventLoop* Next();
private:
	static void* ThreadMain(void* arg);
	std::vector<EventLoop*> loops_;
	std::vector<pthread_t> threads_;
	int num_thread_;
	int cur_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(EventLoopGroup);
};
}}

#endif
