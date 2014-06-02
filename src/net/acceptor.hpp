

#ifndef ABB_NET_ACCEPTOR_H_
#define ABB_NET_ACCEPTOR_H_
#include "ip_addr.hpp"
#include "i_poller_event.hpp"
#include "poller.hpp"
#include "listener.hpp"
namespace abb {
namespace net {
class Loop;
class Connection;
class Context;
class Acceptor :public IPollerEvent{
public:
	explicit Acceptor(Loop* loop);
	void SetListener(IAcceptorListener* lis){lis_ = lis;}
	bool Listen(const IPAddr& addr,int* save_err = NULL);
	void SetEnable(bool benable);
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite(){}
	void Destroy();
private:
	static void StaticDelete(void*arg){
		Acceptor* a = (Acceptor*)arg;
		delete a;
	}
	~Acceptor();
private:
	Loop* loop_;
	PollerEntry* entry_;
	IAcceptorListener* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
	bool bfreed_;
	bool listend_;
};

}
}

#endif /* ACCEPTOR_H_ */
