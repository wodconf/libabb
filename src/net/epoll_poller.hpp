

#ifndef ABB_NET_EPOLL_POLLER_H_
#define ABB_NET_EPOLL_POLLER_H_

#include "i_poller_event.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {

enum{
	POLLER_READ = 0x01,
	POLLER_WRITE = 0x02
};
struct PollerEntry {
	PollerEntry(IPollerEvent* lis):lis_(lis),fd_(-1),event_(0),badd_(true){}
	PollerEntry(int fd,IPollerEvent* lis):lis_(lis),fd_(fd),event_(0),badd_(true){}
	~PollerEntry(){}
	void Emitter(int revent){
		if(event_&revent&POLLER_READ){
			this->lis_->PollerEvent_OnRead();
		}
		if(event_&revent&POLLER_WRITE){
			this->lis_->PollerEvent_OnWrite();
		}
	}
	void SetFd(int fd){fd_ = fd;}
	IPollerEvent* lis_;
	int fd_;
	bool badd_;
	int event_;
};
class Poller {
public:
	
public:
	Poller();
	~Poller();
	void AddRead(PollerEntry* arg);
	void DelRead(PollerEntry* arg);
	void AddWrite(PollerEntry* arg);
	void DelWrite(PollerEntry* arg);
	void AddReadWrite(PollerEntry* arg);
	void DelReadWrite(PollerEntry* arg);
	void Poll(int timeout);
private:
	bool SetEvent(int fd,int event,PollerEntry* arg,bool bneedadd);
private:
	int error_;
	int efd_;
};

} /* namespace base */
} /* namespace abb */

#endif /* POLLER_H_ */
