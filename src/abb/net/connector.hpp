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
	void Connect(const IPAddr& addr);
	void Reset();
	void SetListener(IConnectorListener* lis){lis_=lis;}
	void Destroy();
	const IPAddr& GetIpAddr(){
		return addr_;
	}
	EventLoop* GetEventLoop(){
		return loop_;
	}
private:
	void RealConnect();
	void RealReset();
	virtual void HandleEvent(int event);
	virtual ~Connector();
	static void StaticDelete(void*arg){
		Connector* c = (Connector*)arg;
		delete c;
	}
	static void StaticConnect(void* arg){
		Connector* c = (Connector*)arg;
		c->RealConnect();
	}
	static void StaticReset(void* arg){
		Connector* c = (Connector*)arg;
		c->RealReset();
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
