#ifndef __ABB_NET_ACCEPT_REF_HPP__
#define __ABB_NET_ACCEPT_REF_HPP__

#include "abb/base/ref_object.hpp"
#include "abb/net/ip_addr.hpp"
namespace abb {
namespace net {
class Acceptor;
class AcceptorRef :public base::RefObject{
public:
	explicit AcceptorRef(Acceptor* actor);
	virtual ~AcceptorRef();
	const IPAddr& GetAddr();
	bool Close();
private:
	Acceptor* actor_;
};

} /* namespace net */
} /* namespace abb */
#endif
