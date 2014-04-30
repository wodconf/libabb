

#ifndef ABB_NET_ACCEPTOR_H_
#define ABB_NET_ACCEPTOR_H_
#include "abb/net/ip_addr.hpp"
#include "i_poller_event.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/base/thread_pool.hpp"
#include "poller.hpp"
namespace abb {
namespace net {
class Loop;
class Connection;
class Context;
class Acceptor :public IPollerEvent,private base::RefObject{
public:
	static Acceptor* Create(Context* ctx){
		return new Acceptor(ctx);
	}
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Acceptor_Event(Acceptor*,Connection*) = 0;
	};

	void SetEventCallback(IEvent* lis){
		lis_ = lis;
	}
	bool Bind(const IPAddr& addr);
	void SetEnable(bool benable);
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
	void Destroy();
private:
	static void StaticDelete(void*arg){
		Acceptor* a = (Acceptor*)arg;
		a->UnRef();
	}
	explicit Acceptor(Context* ctx);
	~Acceptor();
	void Dispatch(Connection* conn);
private:
	struct DispatchRunner:public base::ThreadPool::Runer{
		DispatchRunner(Acceptor* a,Connection* c):ac(a),conn(c){}
		virtual ~DispatchRunner(){}
		virtual void Execute();
		Connection* conn;
		Acceptor* ac;
	};
	Loop& loop_;
	Poller::Entry entry_;
	IEvent* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
	bool bfreed_;
	Context* ctx_;
};

}
}

#endif /* ACCEPTOR_H_ */
