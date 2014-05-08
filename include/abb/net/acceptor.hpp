

#ifndef ABB_NET_ACCEPTOR_H_
#define ABB_NET_ACCEPTOR_H_
#include "ip_addr.hpp"
#include "i_poller_event.hpp"
#include "../base/ref_object.hpp"
#include "poller.hpp"
#include "../base/callback.hpp"
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
	struct Listener{
			virtual ~Listener(){};
			virtual void L_Acceptor_OnConnection(Connection*) = 0;
		};

	void SetListener(Listener* lis){
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
	Loop& loop_;
	Poller::Entry entry_;
	Listener* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
	bool bfreed_;
	Context* ctx_;
};

}
}

#endif /* ACCEPTOR_H_ */
