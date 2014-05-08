#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connector.hpp>
#include <abb/net/connection.hpp>
#include "protocol.hpp"
class ConnectCB:public abb::net::Connection::Listener{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn),index(0){
		conn->Listener(this);
		this->Send();
	}
	virtual ~ConnectCB(){}
	virtual void L_Connection_OnMessage(Connection* con,void* msg){
		this->Send();
	}
	virtual void L_Connection_OnClose(Connection* con){
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
class EventCb:public abb::net::Connector::Listener{
public:
	virtual ~EventCb(){}
	virtual void L_Connector_OnOpen(abb::net::Connection* conn){
		LOG(INFO)<< "Connector_Open";
		new ConnectCB(conn);
	}
	virtual void L_Connector_OnOpenFail(int err){
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
	ac->SetListener(&ev);
	if( !ac->Connect(addr) ){
		LOG(INFO) << "Connect fail";
		return -1;
	}
	abb::RunContext(ctx);
}
