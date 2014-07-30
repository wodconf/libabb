
#ifndef CONNECTION_REF_HPP_
#define CONNECTION_REF_HPP_
#include "abb/base/ref_object.hpp"
#include "abb/base/buffer.hpp"
#include "abb/net/ip_addr.hpp"
namespace abb {
namespace net {
class Connection;
class ConnectionRef :public base::RefObject{
public:
	explicit ConnectionRef(Connection* conn);
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
private:
	Connection* conn_;
	IPAddr local_;
	IPAddr remote_;
};

} /* namespace net */
} /* namespace abb */

#endif /* CONNECTION_REF_HPP_ */
