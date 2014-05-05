#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_

#include "../base/callback.hpp"
#include "../base/ref_object.hpp"
#include "poller.hpp"
#include "ip_addr.hpp"

namespace abb{
namespace net{
class Connection;
class Loop;
class Context;
class Connector:public IPollerEvent,private base::RefObject{
public:
	static Connector* Create(Context* ctx){
		return new Connector(ctx);
	}
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Connector_Open(Connection* )=0;
		virtual void Connector_OpenFail(int err)=0;
	};
	
	bool Connect(const IPAddr& addr,int timeout=3000);
	void Reset();
	void SetEventCallback(IEvent* ev){lis_=ev;}
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
	void Destroy();
private:
	Connector(Context* ctx);
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		c->UnRef();
	}
private:
	struct DispatchRunner:public base::CallBack{
		virtual ~DispatchRunner(){}
		virtual void Call();
		Connection* conn;
		Connector* self;
		int err;
	};
	friend struct DispatchRunner;
	bool bfree;
	int fd_;
	IEvent* lis_;
	IPAddr addr_;
	Poller::Entry entry_;
	Loop& loop_;
	Context* ctx_;
};
}
}
#endif
