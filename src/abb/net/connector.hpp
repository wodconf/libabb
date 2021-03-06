#ifndef ABB_NET_CONNECTOR_HPP_
#define ABB_NET_CONNECTOR_HPP_


#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/base/define.hpp"
#include "abb/net/event_handler.hpp"
#include "abb/net/event_io.hpp"
namespace abb{
namespace net{
class EventLoop;
class PollerEntry;
class Connector:public IEventHandler{
public:
	Connector(EventLoop* loop);
	void Connect(const SocketAddress& addr);
	void Reset();
	void SetListener(IConnectorListener* lis){lis_=lis;}
	void Destroy();
	const SocketAddress& GetIpAddr() const{return addr_;}
	EventLoop* GetEventLoop(){return io_event_.GetEventLoop();}
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
	IConnectorListener* lis_;
	SocketAddress addr_;
	IOEvent io_event_;
	int connecting_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connector);
};
}
}
#endif
