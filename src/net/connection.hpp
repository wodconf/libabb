

#ifndef ABB_NET_CONNECTION_H_
#define ABB_NET_CONNECTION_H_

#include "i_poller_event.hpp"
#include "abb/base/callback.hpp"
#include "abb/base/mutex.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "poller.hpp"
#include <sys/socket.h>
namespace abb {
namespace net {
class Loop;
class PollerEntry;
class Connection :public IPollerEvent{
public:
	Connection(Loop* loop,int fd,const IPAddr& local,const IPAddr& peer,int id);
	void SetListener(IConnectionListener* lis){lis_ = lis;}
	base::Buffer& LockWrite();
	void UnLockWrite();
	void SendData(void*buf,unsigned int size);
	int ShutDown(int how = SHUT_RDWR){return shutdown(this->fd_,how);}
	bool IsConnected(){return this->state_ == STATE_OPEN;}
	int GetError(){return this->err_;}
	int GetId(){return id_;}
	void Destroy();
	void SetData(void*data){data_ = data;}
	void* GetData(){return data_;}

	void SetEnable(bool enable);
private:
	virtual void PollerEvent_OnRead();
		virtual void PollerEvent_OnWrite();
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
		delete con;
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
	bool enable_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer wr_buf_;
	Loop* loop_;
	PollerEntry entry_;
	IConnectionListener* lis_;
	int id_;
	void* data_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
