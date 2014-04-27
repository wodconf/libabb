#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_
#include "abb/net/ip_addr.hpp"
#include "poller.hpp"
namespace abb{
namespace net{
class Connection;
class Connector:public IPollerEvent{
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Connector_Open(Connection* conn)=0;
		virtual void Connector_OpenFail(int err)=0;
	};
	Connector();
	virtual ~Connector();
	bool Connect(const IPAddr& addr);
	void Reset();
	void SetEventCallback(IEvent* ev){lis_=ev;}
	virtual void PollerEvent_OnRead(){}
	virtual void PollerEvent_OnWrite();
private:
	int fd_;
	IEvent* lis_;
	IPAddr addr_;
	Poller::Entry entry_;
	Poller& poller_;
};
}
}
#endif
