

#ifndef __ABB_NET_CONNECTION_H__
#define __ABB_NET_CONNECTION_H__

#include "abb/base/callback.hpp"
#include "abb/base/mutex.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
#include "abb/net/ip_addr.hpp"
#include "abb/net/listener.hpp"
#include "abb/net/event_handler.hpp"
#include "abb/net/event_io.hpp"

namespace abb {
namespace net {
class EventLoop;

class Connection :public IEventHandler{
public:
	Connection(EventLoop* loop,int fd,const IPAddr& local,const IPAddr& peer);
	void SetListener(IConnectionListener* lis){lis_ = lis;}
	void SetNoDelay(bool e);
	void Start();
	bool LockWrite(base::Buffer**buf);
	void UnLockWrite(bool bflush);
	void Write(void*buf,unsigned int size);
	void WriteAndFlush(void*buf,unsigned int size);
	void Flush();
	void ShutDown(bool read,bool write);
	void ShutDownAfterWrite();
	void Destroy();

	bool IsConnected(){return this->state_ == STATE_OPEN && !shut_down_after_write_;}
	const IPAddr& GetLocalAddr(){return local_addr_;}
	const IPAddr& GetRemoteAddr(){return peer_addr_;}
	EventLoop* GetEventLoop(){
		return io_event_.GetEventLoop();
	}
	void SetData(void*data){data_ = data;}
	void* GetData(){return data_;}
private:
	void InternalFlush();
	virtual void HandleEvent(int event);
	void OnRead();
	void OnWrite();
	
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
	}state_;
	int fd_;
	int err_;
	IPAddr local_addr_;
	IPAddr peer_addr_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer* wr_buf_;
	base::Buffer* wring_buf_;
	base::Buffer wr_buf_1_;
	base::Buffer wr_buf_2_;
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
