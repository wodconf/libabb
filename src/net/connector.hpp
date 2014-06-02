#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_

#include "../base/callback.hpp"
#include "../base/ref_object.hpp"
#include "poller.hpp"
#include "ip_addr.hpp"
#include "listener.hpp"
namespace abb{
namespace net{
class Connection;
class Loop;
class Context;
class PollerEntry;
class Connector:public IPollerEvent{
public:
	Connector(Loop* loop);
	bool Connect(const IPAddr& addr,int* save_error);
	void Reset();
	void SetListener(IConnectorListener* lis){lis_=lis;}
	void Destroy();
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
private:
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		delete c;
	}
private:
	bool bfree;
	int fd_;
	IConnectorListener* lis_;
	IPAddr addr_;
	PollerEntry* entry_;
	Loop* loop_;
};
}
}
#endif
