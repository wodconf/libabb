#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connector.hpp>
#include <abb/net/connection.hpp>
class ConnectCB:public abb::net::Connection::IEvent{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn){
		conn->SetEnable(true);
		conn->SetEventCallBack(this);
		this->Send();
	}
	virtual ~ConnectCB(){}
	virtual void Connection_Event(int ev){
		LOG(INFO)<< ev;
		if(ev == abb::net::Connection::EVENT_READ){
			abb::base::Buffer&buf = conn->LockRead();
			buf.Clear();
			conn->UnLockRead();
		}
		this->Send();
		LOG(INFO)<< "Send";
	}
	void Send(){
		char buf[] = "hello";
		LOG(INFO) << conn->Write(buf,sizeof(buf),NULL);
	}
	abb::net::Connection* conn;
};
class EventCb:public abb::net::Connector::IEvent{
public:
	virtual ~EventCb(){}
	virtual void Connector_Open(abb::net::Connection* conn){
		LOG(INFO)<< "Connector_Open";
		new ConnectCB(conn);
	}
	virtual void Connector_OpenFail(int err){
		LOG(INFO)<< "Connector_OpenFail" << err << strerror(err);
	}
};
int main(){
	abb::net::Context* ctx = abb::NewContext();
	LOG(INFO) << "c";
	abb::net::IPAddr addr;
	if( ! addr.SetV4("127.0.0.1",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::net::Connector* ac = abb::net::Connector::Create(ctx);
	EventCb ev;
	ac->SetEventCallback(&ev);
	if( !ac->Connect(addr) ){
		LOG(INFO) << "Connect fail";
		return -1;
	}
	abb::RunContext(ctx,true);
}
