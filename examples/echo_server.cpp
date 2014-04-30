

#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connection.hpp>
class ConnectCB:public abb::net::Connection::IEvent{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn){
		conn->SetEnable(true);
		conn->SetEventCallBack(this);
	}
	virtual ~ConnectCB(){}
	virtual void Connection_Event(int ev){
		LOG(INFO)<< ev;
		if(ev == abb::net::Connection::EVENT_READ){
			abb::base::Buffer buf = conn->LockRead();
			buf.Clear();
			conn->UnLockRead();
		}
		this->Send();
	}
	void Send(){
		char buf[] = "world";
		conn->Write(buf,sizeof(buf),NULL);
	}
	abb::net::Connection* conn;
};
class EventCb:public abb::net::Acceptor::IEvent{
public:
	virtual ~EventCb(){}
	virtual void Acceptor_Event(abb::net::Acceptor*,abb::net::Connection* c){
		new ConnectCB(c);
	}
};
int main(){
	abb::net::Context* ctx = abb::NewContext();
	abb::net::IPAddr addr;
	if( ! addr.SetV4(NULL,9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::net::Acceptor* ac = abb::net::Acceptor::Create(ctx);
	EventCb ev;
	ac->SetEventCallback(&ev);
	if( !ac->Bind(addr) ){
		LOG(INFO) << "Bind fail";
		return -1;
	}

	ac->SetEnable(true);
	abb::RunContext(ctx,true);
}
