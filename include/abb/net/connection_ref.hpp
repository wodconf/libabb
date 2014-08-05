
#ifndef __ABB_NET_CONNECTION_REF_HPP__
#define __ABB_NET_CONNECTION_REF_HPP__

#include "abb/base/ref_object.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
#include "abb/net/ip_addr.hpp"

namespace abb {
namespace net {
class Connection;
class EventLoop;
class ConnectionRef :public base::RefObject{
public:
	explicit ConnectionRef(Connection* conn);
	virtual ~ConnectionRef();
	bool IsClosed();
	bool Send(void*data,int len);
	bool LockWrite(base::Buffer**buf);
	void UnLockWrite();
	void SetNoDelay(bool e);
	const IPAddr& GetLocalAddr(){return local_;}
	const IPAddr& GetRemoteAddr(){return remote_;}
	void Close();
	void CloseAfterWrite();
	void* Data;
	EventLoop* GetEventLoop();
private:
	Connection* conn_;
	IPAddr local_;
	IPAddr remote_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(ConnectionRef);
};

} /* namespace net */
} /* namespace abb */

#endif /* CONNECTION_REF_HPP_ */
