
#ifndef CONNECTION_REF_HPP_
#define CONNECTION_REF_HPP_
#include "../base/ref_object.hpp"
#include "../base/mutex.hpp"
#include "../base/buffer.hpp"
namespace abb {
namespace net {
class Connection;
class ConnectionRef :public base::RefObject{
public:
	ConnectionRef(Connection* conn);
	virtual ~ConnectionRef();
	bool IsClosed(){return conn_==NULL;}
	bool Send(void*data,int len);
	bool LockWrite(base::Buffer**buf);
	void UnLockWrite();
	void Close();
	void* Data;
private:
	friend class TcpServer;
	friend class TcpClient;
	bool TestAndSetNULL(){
		base::Mutex::Locker l(mtx_);
		bool b = conn_ == NULL;
		conn_ = NULL;
		return b;
	}
	Connection* conn_;
	base::Mutex mtx_;
	bool block_write_;
};

} /* namespace net */
} /* namespace abb */

#endif /* CONNECTION_REF_HPP_ */
