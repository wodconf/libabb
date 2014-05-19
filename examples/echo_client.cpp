#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connector.hpp>
#include <abb/net/connection.hpp>
class ConnectCB:public abb::net::Connection::IEvent{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn),index(0){
		conn->SetEventCallback(this);
		this->Send();
	}
	virtual ~ConnectCB(){}
	virtual void L_Connection_EventRead(abb::net::Connection* self,abb::base::Buffer& buf){
		buf.Clear();
		this->Send();
	}
	virtual void L_Connection_EventClose(abb::net::Connection* self){
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
	virtual void L_Connector_EventOpen(abb::net::Connection* conn){
		LOG(INFO)<< "Connector_Open";
		new ConnectCB(conn);
	}
	virtual void L_Connector_EventError(int err){
		LOG(INFO)<< "Connector_OpenFail" << err << strerror(err);
	}
};
int main(){
	abb::net::ContextOption option;
	option.SetRunCurrentThread(true);
	abb::net::Context* ctx = abb::NewContext(option);
	LOG(INFO) << "c";
	abb::net::IPAddr addr;
	if( ! addr.SetV4("127.0.0.1",9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::net::Connector* ac = abb::net::Connector::Create(ctx);
	EventCb ev;
	ac->SetEventCallback(&ev);
	int err;
	if( !ac->Connect(addr,&err) ){
		LOG(INFO) << "Connect fail" << strerror(err);
		return -1;
	}
	abb::RunContext(ctx);
}
