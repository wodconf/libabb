

#ifndef __ABB_NET_CONNECTION_H__
#define __ABB_NET_CONNECTION_H__

#include "abb/base/callback.hpp"
#include "abb/base/thread.hpp"
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
	void Established();
	bool LockWrite(Buffer**buf);
	void UnLockWrite(bool bflush);
	void Write(void*buf,unsigned int size);
	void WriteAndFlush(void*buf,unsigned int size);
	void Flush();
	void CloseAfterWrite();
	void Destroy();

	bool IsConnected(){
		return ( __sync_bool_compare_and_swap(&this->close_,0,0)
				&& __sync_bool_compare_and_swap(&this->shut_down_after_write_,0,0) );
	}
	const IPAddr& GetLocalAddr(){return local_addr_;}
	const IPAddr& GetRemoteAddr(){return peer_addr_;}
	EventLoop* GetEventLoop(){
		return io_event_.GetEventLoop();
	}
	void SetData(void*data){data_ = data;}
	void* GetData(){return data_;}
private:
	void ShutDown(bool read,bool write);
	void InternalFlush();
	virtual void HandleEvent(int event);
	void LoopedEstablished();
	void OnRead();
	void OnWrite();
	void LoopedAllowWrite();
	static void StaticEstablished(void*arg){
		Connection* con = (Connection*)arg;
		con->LoopedEstablished();
	}
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
	static void StaticAllowWrite(void *arg){
		Connection* con = (Connection*)arg;
		con->LoopedAllowWrite();
	}
	static void StaticCloseTimeout(void* arg){
		Connection* con = (Connection*)arg;
		con->LoopTimeout();
	}
	int Reader(const struct iovec *iov, int iovcnt);
	int Writer(void*buf,int size);
	void LoopTimeout();
private:
	virtual ~Connection();
	
	IOEvent io_event_;
	IPAddr local_addr_;
	IPAddr peer_addr_;
	IConnectionListener* lis_;
	int err_;
	int close_;
	void* data_;
	int shut_down_after_write_;
	bool block_write_;

	Buffer rd_buf_;
	Mutex wr_lock_;
	Buffer* wr_buf_;
	Buffer* wring_buf_;
	Buffer wr_buf_1_;
	Buffer wr_buf_2_;

	int time_out_id_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
