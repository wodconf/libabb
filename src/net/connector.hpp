#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_

#include "poller.hpp"
#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/base/define.hpp"
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
	bool Reset();
	void SetListener(IConnectorListener* lis){lis_=lis;}
	void Destroy();
	const IPAddr& GetIpAddr(){
		return addr_;
	}
private:
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		delete c;
	}
private:
	int fd_;
	IConnectorListener* lis_;
	IPAddr addr_;
	PollerEntry entry_;
	Loop* loop_;
	int connected_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connector);
};
}
}
#endif
