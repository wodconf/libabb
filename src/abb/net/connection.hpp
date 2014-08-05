

#ifndef ABB_NET_CONNECTION_H_
#define ABB_NET_CONNECTION_H_

#include "abb/base/callback.hpp"
#include "abb/base/mutex.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/net/event_handler.hpp"
#include "abb/net/io_event.hpp"
#include "poller.hpp"
#include <sys/socket.h>

namespace abb {
namespace net {
class EventLoop;

class Connection :public IEventHandler{
public:
	Connection(EventLoop* loop,int fd,const IPAddr& local,const IPAddr& peer);
	void SetListener(IConnectionListener* lis){lis_ = lis;}
	bool LockWrite(base::Buffer**buf);
	void UnLockWrite();
	void SendData(void*buf,unsigned int size);
	int ShutDown(int how = SHUT_RDWR){LOG(INFO) << "shutdown1";return shutdown(this->fd_,how);}
	void ShutDownAfterWrite();
	bool IsConnected(){return this->state_ == STATE_OPEN;}
	int GetError(){return this->err_;}
	void Destroy();
	void SetData(void*data){data_ = data;}
	void* GetData(){return data_;}

	void SetEnable(bool enable);
	void SetNoDelay(bool e);
	const IPAddr& GetLocalAddr(){return local_addr_;}
	const IPAddr& GetRemoteAddr(){return peer_addr_;}
	bool SetKeepAlive(bool kp,int idle,int interval,int cout);
	EventLoop* GetEventLoop(){
		return loop_;
	}
private:
	virtual void HandleEvent(int event);
	void OnRead();
	void OnWrite();
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
	IPAddr local_addr_;
	IPAddr peer_addr_;
	bool enable_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer* wr_buf_;
	base::Buffer* wring_buf_;
	base::Buffer wr_buf_1_;
	base::Buffer wr_buf_2_;
	EventLoop* loop_;
	IOEvent io_event_;
	bool block_write_;
	IConnectionListener* lis_;
	void* data_;
	bool shut_down_after_write_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
