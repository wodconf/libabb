#include "abb/net/timer_set.hpp"
#include <sys/time.h>
#include "abb/net/event_loop.hpp"
#include "abb/base/log.hpp"
namespace abb{
namespace net{
static uint64_t MSNow(){
	struct timeval time;
	gettimeofday(&time,0);
	uint64_t tmp =  time.tv_sec;
	tmp*=1000;
	tmp+=time.tv_usec/1000;
	return tmp;
}
TimerSet::TimerSet(EventLoop* loop):loop_(loop),statrt_id_(1){

}
TimerSet::~TimerSet(){
	IDMap::iterator iter = id_map_.begin();
	for(;iter!=id_map_.end();iter++){
		delete iter->second;
	}
	id_map_.clear();
	timer_map_.clear();
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
	t->remove_ = false;
	loop_->RunInLoop(LoopAddTimer,t);
	return id;
}
void TimerSet::LoopAddTimer(void* timer){
	Timer* t = static_cast<Timer*>(timer);
	t->timer_set_->id_map_[t->id_] = t;
	t->timer_set_->timer_map_.insert(std::make_pair(t->pending_time_,t));
}
void TimerSet::LoopRemoveTimer(void* arg){
	Arg* a = static_cast<Arg*>(arg);
	IDMap::iterator iter = a->timer_set_->id_map_.find(a->id_);
	if(iter != a->timer_set_->id_map_.end()){
		a->timer_set_->id_map_.erase(iter);
		iter->second->remove_ = true;
		TimerProcessMap::iterator low = a->timer_set_->timer_map_.lower_bound(iter->second->pending_time_);
		TimerProcessMap::iterator up = a->timer_set_->timer_map_.upper_bound(iter->second->pending_time_);
		while (low != up){
		   if(low->second == iter->second){
		   		a->timer_set_->timer_map_.erase(low);
		   		delete low->second;
		   		break;
		   }
		    ++low;
		}
	}
	delete a;
}
void TimerSet::RemoveTimer(int id){
	Arg* arg = new Arg();
	arg->id_ = id;
	arg->timer_set_ = this;
	loop_->RunInLoop(LoopRemoveTimer,arg);
}
int TimerSet::Process(){
	int min = -1;
	while(timer_map_.size() > 0){
		TimerProcessMap::iterator iter = timer_map_.begin();
		uint64_t now = MSNow();
		if(iter->first <= now){
			timer_map_.erase(iter);
			Timer* t = iter->second;
			t->fn_(t->arg_);
			if(t->remove_){
				delete t;
			}else if(t->repeat_){
				t->pending_time_ = now + t->timeout_ ;
				timer_map_.insert(std::make_pair(t->pending_time_,t));
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
