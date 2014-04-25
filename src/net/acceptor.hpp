
 
#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_
#include "abb/net/addr.hpp"
#include "poller_able.hpp"
namespace abb {
namespace net {
class Connection;
class Acceptor :public PollerAble{
public:
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void Acceptor_Event(Acceptor*,Connection*) = 0;
	};
	Acceptor(IEvent* lis);
	~Acceptor();
	bool Bind(const SockAddr& addr);
	void SetEnable(bool benable);
private:
	struct Dispatch:public base::ThreadPool::Runer{
		virtual ~Dispatch(){}
		virtual void Execute();
		Acceptor* self;
	};
	friend struct Dispatch;
	Dispatch dis_;
	virtual void OnEvent(int event);
	IEvent* lis_;
	bool enable_;
	SockAddr addr_;
	int fd_;
};

}
}

#endif /* ACCEPTOR_H_ */
