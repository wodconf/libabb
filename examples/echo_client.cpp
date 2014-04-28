#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connector.hpp>
#include <abb/net/connection.hpp>
class ConnectCB:public abb::net::Connection::IEvent{
public:
	virtual ~ConnectCB(){}
	virtual void Connection_Event(int ev){
		LOG(INFO)<< ev;
	}
};
class EventCb:public abb::net::Connector::IEvent{
public:
	virtual ~EventCb(){}
	virtual void Connector_Open(abb::net::Connection* conn){
		LOG(INFO)<< "Connector_Open";
		conn->SetEnable(true);
		conn->SetEventCallBack(new ConnectCB());
	}
	virtual void Connector_OpenFail(int err){
		LOG(INFO)<< "Connector_OpenFail" << err << strerror(err);
	}
};
int main(){
	LOG(INFO) << "init";
	abb::Init();
	LOG(INFO) << "c";
	abb::net::IPAddr addr;
	if( ! addr.SetV4("127.0.0.1",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::net::Connector* ac = abb::net::Connector::New();
	EventCb ev;
	ac->SetEventCallback(&ev);
	if( !ac->Connect(addr) ){
		LOG(INFO) << "Connect fail";
		return -1;
	}
	abb::Run();
}
