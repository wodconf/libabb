
#ifndef __ABB_NET_CONNECTION_REF_HPP__
#define __ABB_NET_CONNECTION_REF_HPP__
#include "abb/base/ref_object.hpp"
#include "abb/base/buffer.hpp"
#include "abb/net/ip_addr.hpp"
namespace abb {
namespace net {
class Connection;
class AcceptorRef;
class ConnectionRef :public base::RefObject{
public:
	explicit ConnectionRef(Connection* conn,AcceptorRef* parent);
	virtual ~ConnectionRef();
	bool IsClosed();
	bool Send(void*data,int len);
	bool LockWrite(base::Buffer**buf);
	void UnLockWrite();
	void SetNoDelay(bool e);
	void SetKeepAlive(bool kp,int idle,int interval,int cout);
	const IPAddr& GetLocalAddr(){return local_;}
	const IPAddr& GetRemoteAddr(){return remote_;}
	void Close();
	void CloseAfterWrite();
	void* Data;
	AcceptorRef* Parent(){return parent_;}
private:
	AcceptorRef* parent_;
	Connection* conn_;
	IPAddr local_;
	IPAddr remote_;
};

} /* namespace net */
} /* namespace abb */

#endif /* CONNECTION_REF_HPP_ */
