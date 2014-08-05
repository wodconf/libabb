#include "abb/net/timer_set.hpp"


namespace abb{
namespace net{
static uint64_t MSNow(){
	struct timeval time;
	gettimeofday(&time,NULL);
	uint64_t tmp =  time.tv_sec;
	tmp*=1000;
	tmp+=time.tv_usec/1000;
	return tmp;
}
	int TimerSet::AddTimer(int ms,bool repeat,run_fn fn,void*arg){
		int id = NextId();
		Timer* t = new Timer();
		t->id_ = id;
		t->arg_ =arg;
		t->fn_ = fn;
		t->pending_time_ = MSNow() + ms;
		t->timeout_ = ms;
		t->repeat_ = repeat;
		t->timer_set_ = this;
		loop->RunInLoop(LoopAddTimer,t);
		return id;
	}
	void TimerSet::LoopAddTimer(void* timer){
		Timer* t = static_cast<Timer*>(timer);
		t->timer_set_->id_map_[t->id_] = t;
		t->timer_set_->timer_map_[t->pending_time_] = t;
	}
	void TimerSet::LoopRemoveTimer(void* arg){
		Arg* a = static_cast<Arg*>(arg);
		IDMap::iterator iter = a->timer_set_->id_map_.find(a->id_);
		if(iter != a->timer_set_->id_map_.end()){
			a->timer_set_->timer_map_.erase(iter->second->pending_time_);
			a->timer_set_->id_map_.erase(iter);
		}
		delete a;
	}
	void TimerSet::RemoveTimer(int id){
		Arg* arg = new Arg();
		arg->id_ = id;
		Arg->timer_set_ = this;
		loop->RunInLoop(LoopAddTimer,arg);
	}
	int TimerSet::Process(){
		int min = -1;
		while(timer_map_.size() > 0){
			TimerProcessMap::iterator iter = timer_map_.begin();
			uint64_t now = MSNow();
			if(iter->first < now){
				timer_map_.erase(iter);
				Timer* t = iter->second;
				t->fn_(t->arg_);
				if(t->repeat_){
					t->pending_time_ = now + t->timeout_ ;
					timer_map_[t->pending_time_] = t;
				}else{
					id_map_.erase(t->id_);
					delete t;
				}
			}else{
				min = iter->first - now;
				break;
			}
		}
		return min;
	}
}
}