
 
#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_
#include "abb/net/ip_addr.hpp"
#include "i_poller_event.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/base/thread_pool.hpp"
#include "poller.hpp"
namespace abb {
namespace net {
class Loop;
class Connection;
class Acceptor :public IPollerEvent,private base::RefObject{
public:
	static Acceptor* New(){
		return new Acceptor();
	}
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Acceptor_Event(Acceptor*,Connection*) = 0;
	};
	
	void SetEventCallback(IEvent* lis){
		lis_ = lis;
	}
	bool Bind(const IPAddr& addr);
	void SetEnable(bool benable);
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
	void Delete();
private:
	static void StaticDelete(void*arg){
		Acceptor* a = (Acceptor*)arg;
		a->UnRef();
	}
	Acceptor();
	~Acceptor();
	void Dispatch(Connection* conn);
private:
	struct DispatchRunner:public base::ThreadPool::Runer{
		virtual ~DispatchRunner(){}
		virtual void Execute();
		Connection* conn;
		Acceptor* ac;
	};
	base::ThreadPool& dispatch_;
	Loop& loop_;
	Poller::Entry entry_;
	IEvent* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
	bool bfreed_;
};

}
}

#endif /* ACCEPTOR_H_ */
