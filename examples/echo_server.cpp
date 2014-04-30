

#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
#include <abb/net/connection.hpp>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
void sleep(int ms){
	struct timeval tv;
	tv.tv_sec = ms/1000;
	tv.tv_usec = ( ms- tv.tv_sec*1000)*1000;
	select(0,0,0,0,&tv);
}
int num_pkt = 0;
class ConnectCB:public abb::net::Connection::IEvent{
public:
	ConnectCB(abb::net::Connection*conn):conn(conn),index(0){
		conn->SetEnable(true);
		conn->SetEventCallBack(this);
	}
	virtual ~ConnectCB(){}
	virtual void Connection_Event(int ev){
		if(ev == abb::net::Connection::EVENT_READ){
			abb::base::Buffer&buf = conn->LockRead();
			buf.Clear();
			conn->UnLockRead();
			num_pkt++;
		}
		this->Send();
	}
	void Send(){
		index++;
		conn->Write(&index,sizeof(index),NULL);
	}
	int index ;
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
	abb::RunContext(ctx,false);
	while(true){
		if(num_pkt)
			LOG(INFO) << "num_pkt:" << num_pkt;
		num_pkt = 0;
		sleep(1000);
	}
}
