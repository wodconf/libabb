/*
 * Acceptor.h
 *
 *  Created on: 2014Äê4ÔÂ25ÈÕ
 *      Author: goalworld
 */

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
	Acceptor();
	~Acceptor();
	bool Bind(const SockAddr& addr);
	void SetEnable(bool benable);
private:
	virtual void OnEvent(int event);
	bool enable_;
	SockAddr addr_;
	int fd_;
};

}
}

#endif /* ACCEPTOR_H_ */
