#ifndef __ABB_NET_TIMER_SET_HPP__
#define __ABB_NET_TIMER_SET_HPP__

#include <stdint.h>
#include <map>

namespace abb{
namespace net{
class EventLoop;
class TimerSet{
public:
	typedef void(*run_fn)(void* arg);
	TimerSet(EventLoop* loop);
	~TimerSet();
	int AddTimer(int ms,bool brepeat,run_fn fn,void*arg);
	void RemoveTimer(int id);
	int Process();
private:
	static void LoopAddTimer(void* timer);
	static void LoopRemoveTimer(void* arg);
	int NextId(){
		return __sync_add_and_fetch(&id_,1);
	}
	struct Timer{
		
		int id_;
		void*arg_;
		run_fn fn_;
		uint64_t pending_time_;
		int timeout_;
		bool repeat_;
		TimerSet* timer_set_;
	};
	struct Arg{
		TimerSet* timer_set_;
		int id_;
	};
	typedef std::map<uint64_t,Timer*> TimerProcessMap;
	typedef std::map<int,Timer*> IDMap;
	IDMap id_map_;
	TimerProcessMap timer_map_;
	EventLoop* loop_;
	int id_;
};
}
}

#endif
