

#ifndef ABB_NET_CONNECTION_H_
#define ABB_NET_CONNECTION_H_

#include "i_poller_event.hpp"
#include "../base/ref_object.hpp"
#include "../base/callback.hpp"
#include "../base/mutex.hpp"
#include "../base/rw_lock.hpp"
#include "../base/buffer.hpp"
#include "../base/define.hpp"
#include "../net/ip_addr.hpp"
#include "../base/gcc_mutex.hpp"
#include "poller.hpp"
#include <sys/socket.h>
namespace abb {
namespace net {
class Loop;
class PollerEntry;
class Connection :public IPollerEvent,public base::RefObject{
public:
	Connection(Loop* loop,int fd,const IPAddr& local,const IPAddr& peer);
	void SetListener(IEvent* event){ev_ = event;}
	base::Buffer& LockWrite();
	void UnLockWrite();
	void SendData(void*buf,unsigned int size);
	int ShutDown(int how = SHUT_RDWR){
		return shutdown(this->fd_,how);
	}
	void Destroy();
	bool IsConnected(){
		return this->state_ == STATE_OPEN;
	}
	int GetError(){
		return this->err_;
	}
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
private:
	void Flush();
	static int StaticReader(void*arg,const struct iovec *iov, int iovcnt){
		Connection* con = (Connection*)arg;
		return con->Reader(iov,iovcnt);
	}
	static int StaticWriter(void*arg,void*buf,int size){
		Connection* con = (Connection*)arg;
		return con->Writer(buf,size);
	}
	static void StaticFree(void*arg){
		Connection* con = (Connection*)arg;
		con->UnRef();
	}
	int Reader(const struct iovec *iov, int iovcnt);
	int Writer(void*buf,int size);
private:
	virtual ~Connection();
	enum{
		STATE_OPEN,
		STATE_CLOSE,
		STATE_ERROR
	}state_;
	int fd_;
	int err_;
	int bfreed_;
	IPAddr local_addr_;
	IPAddr peer_addr_;
	
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer wr_buf_;
	Loop* loop_;
	PollerEntry* entry_;
	IConnectionListener* lis_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
