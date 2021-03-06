#ifndef __ABB_NET_TIMER_SET_HPP__
#define __ABB_NET_TIMER_SET_HPP__

#include <stdint.h>
#include <map>
#include "abb/base/ref_object.hpp"
namespace abb{
namespace net{
class EventLoop;
class TimerSet{
public:
	typedef void(*run_fn)(void* arg,int id);
	TimerSet(EventLoop* loop);
	~TimerSet();
	int AddTimer(int ms,bool brepeat,run_fn fn,void*arg);
	void RemoveTimer(int id);
	int Process();
private:
	static void LoopAddTimer(void* timer);
	static void LoopRemoveTimer(void* arg);
	int NextId(){
		return __sync_add_and_fetch(&statrt_id_,1);
	}
	struct Timer{
		int id_;
		void*arg_;
		run_fn fn_;
		uint64_t pending_time_;
		int timeout_;
		bool repeat_;
		TimerSet* timer_set_;
		bool remove_;
	};
	struct Arg{
		TimerSet* timer_set_;
		int id_;
	};
	typedef std::multimap<uint64_t,Timer*> TimerProcessMap;
	typedef std::map<int,Timer*> IDMap;
	IDMap id_map_;
	TimerProcessMap timer_map_;
	EventLoop* loop_;
	int statrt_id_;
};
}
}

#endif
