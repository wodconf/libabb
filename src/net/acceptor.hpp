
 
#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_
#include "abb/net/ip_addr.hpp"
#include "i_poller_event.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/base/thread_pool.hpp"
#include "poller.hpp"
namespace abb {
namespace net {
class Connection;
class Acceptor :public IPollerEvent,base::RefObject{
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Acceptor_Event(Acceptor*,Connection*) = 0;
	};
	Acceptor();
	~Acceptor();
	void SetListener(IEvent* lis){
		lis_ = lis;
	}
	bool Bind(const IPAddr& addr);
	void SetEnable(bool benable);
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
private:
	void Dispatch(Connection* conn);
private:
	struct DispatchRunner:public base::ThreadPool::Runer{
		virtual ~DispatchRunner(){}
		virtual void Execute();
		Connection* conn;
		Acceptor* ac;
	};
	base::ThreadPool& dispatch_;
	Poller& poller_;
	Poller::Entry entry_;
	IEvent* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
};

}
}

#endif /* ACCEPTOR_H_ */
