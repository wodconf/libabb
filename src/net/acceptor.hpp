

#ifndef ABB_NET_ACCEPTOR_H_
#define ABB_NET_ACCEPTOR_H_
#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/base/define.hpp"
#include "abb/net/event_handler.hpp"
#include "abb/net/io_event.hpp"
namespace abb {
namespace net {
class Loop;
class Connection;
class Context;
class Acceptor :public IEventHandler{
public:
	explicit Acceptor(Loop* loop);
	void SetListener(IAcceptorListener* lis){lis_ = lis;}
	bool Listen(const IPAddr& addr,int* save_err = NULL);
	void SetEnable(bool benable);
	void Close();
	void Destroy();
	const IPAddr& GetIpAddr(){
		return addr_;
	}
private:
	virtual void HandleEvent(int event);
	static void StaticDelete(void*arg){
		Acceptor* a = (Acceptor*)arg;
		delete a;
	}
	~Acceptor();
private:
	Loop* loop_;
	IOEvent entry_;
	IAcceptorListener* lis_;
	bool enable_;
	IPAddr addr_;
	int fd_;
	bool bfreed_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Acceptor);
};

}
}

#endif /* ACCEPTOR_H_ */
