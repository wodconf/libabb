#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_


#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/base/define.hpp"
#include "abb/net/event_handler.hpp"
#include "abb/net/io_event.hpp"
namespace abb{
namespace net{
class EventLoop;
class PollerEntry;
class Connector:public IEventHandler{
public:
	Connector(EventLoop* loop);
	bool Connect(const IPAddr& addr,int* save_error);
	bool Reset();
	void SetListener(IConnectorListener* lis){lis_=lis;}
	void Destroy();
	const IPAddr& GetIpAddr(){
		return addr_;
	}
private:
	virtual void HandleEvent(int event);
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		delete c;
	}
private:
	int fd_;
	IConnectorListener* lis_;
	IPAddr addr_;
	IOEvent io_event_;
	EventLoop* loop_;
	int connected_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connector);
};
}
}
#endif
