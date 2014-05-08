#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connector.hpp>
#include <abb/net/connection.hpp>
#include "protocol.hpp"
class ConnectCB:public abb::net::Connection::IEvent{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn),index(0){
		conn->SetEventCallBack(this);
	}
	virtual ~ConnectCB(){}
	virtual void Connection_OnMessage(Connection* con,Msg msg){
		this->Send();
	}
	virtual void Connection_OnClose(Connection* con){
		if(conn->GetError() != 0){
			LOG(INFO)<< "Connection_EventError" << strerror(conn->GetError());
		}else{
			LOG(INFO)<< "Connection_EventClose";
		}
	}
	void Send(){
		index++;
		this->conn->SendData(&index,sizeof(index));
	}
	int index ;
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
	abb::net::ContextOption option;
	option.SetRunCurrentThread(true);
	ProtocolFactory fac;
	abb::net::Context* ctx = abb::NewContext(option,&fac);
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
	abb::RunContext(ctx);
}
