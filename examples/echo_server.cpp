

#include <abb/base/log.hpp>
#include <abb/abb.hpp>
#include <abb/net/acceptor.hpp>
class EventCb:public abb::net::Acceptor::IEvent{
public:
	virtual ~EventCb(){}
	virtual void Acceptor_Event(abb::net::Acceptor*,abb::net::Connection*){
		LOG(INFO) << "a";
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
