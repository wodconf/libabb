#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_

#include "../base/thread_pool.hpp"
#include "../base/ref_object.hpp"
#include "poller.hpp"
#include "ip_addr.hpp"

namespace abb{
namespace net{
class Connection;
class Loop;
class Connector:public IPollerEvent,private base::RefObject{
public:
	static Connector* New(){
		return new Connector();
	}
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Connector_Open(Connection* )=0;
		virtual void Connector_OpenFail(int err)=0;
	};
	
	bool Connect(const IPAddr& addr);
	void Reset();
	void SetEventCallback(IEvent* ev){lis_=ev;}
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
	void Delete();
private:
	Connector();
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		c->UnRef();
	}
private:
	struct DispatchRunner:public base::ThreadPool::Runer{
		virtual ~DispatchRunner(){}
		virtual void Execute();
		Connection* conn;
		Connector* self;
		int err;
	};
	friend struct DispatchRunner;
	bool bfree;
	base::ThreadPool& dispatch_;
	int fd_;
	IEvent* lis_;
	IPAddr addr_;
	Poller::Entry entry_;
	Loop& loop_;
};
}
}
#endif
