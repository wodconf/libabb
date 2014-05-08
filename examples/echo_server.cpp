

#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connection.hpp>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
using namespace abb::net;
void sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
int num_pkt = 0;
class ConnectCB:public abb::net::Connection::Listener{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn),index(0){
		conn->Listener(this);
	}
	virtual ~ConnectCB(){}
	virtual void L_Connection_EventRead(Connection* self,abb::base::Buffer& buf){
		buf.Clear();
		this->Send();
		num_pkt++;
	}
	virtual void L_Connection_EventClose(Connection* self){
		if(conn->GetError() != 0){
			LOG(INFO)<< "Connection_EventError" << strerror(conn->GetError());
		}else{
			LOG(INFO)<< "Connection_EventClose";
		}
	}
	virtual void L_Connection_OnMessage(Connection* con,void* msg){
		this->Send();
		num_pkt++;
	}
	void Send(){
		index++;
		this->conn->SendData(&index,sizeof(index));
	}
	int index ;
	abb::net::Connection* conn;
};
class EventCb:public abb::net::Acceptor::Listener{
public:
	virtual ~EventCb(){}
	virtual void L_Acceptor_Event(abb::net::Connection* c){
		new ConnectCB(c);
	}
};
int main(){
	abb::net::ContextOption option;
	abb::net::Context* ctx = abb::NewContext(option);
	abb::net::IPAddr addr;
	if( ! addr.SetV4(NULL,9922) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::net::Acceptor* ac = abb::net::Acceptor::Create(ctx);
	EventCb ev;
	ac->SetListener(&ev);
	if( !ac->Bind(addr) ){
		LOG(INFO) << "Bind fail";
		return -1;
	}
	LOG(INFO) << "Bind ok";
	ac->SetEnable(true);
	abb::RunContext(ctx);
	while(true){
		if(num_pkt)
			LOG(INFO) << "num_pkt:" << num_pkt;
		num_pkt = 0;
		sleep(1000);
	}
}
