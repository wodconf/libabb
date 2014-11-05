

#ifndef __ABB_NET_CONNECTION_H__
#define __ABB_NET_CONNECTION_H__

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
	Connection(EventLoop* loop,int fd,const SocketAddress& local,const SocketAddress& peer);
	void SetListener(IConnectionListener* lis){lis_ = lis;}
	bool SetNoDelay(bool e,int* error);
	bool SetKeepAlive(bool enable,int delay,int* error);
	void Established();
	bool LockWrite(Buffer**buf);
	void UnLockWrite(bool bflush);
	void Write(const void*data,unsigned int size);
	void WriteAndFlush(const void*data,unsigned int size);
	void Flush();
	void Close();
	void Destroy();
	bool Connected(){return ( __sync_bool_compare_and_swap(&this->close_,0,0)&& __sync_bool_compare_and_swap(&this->shut_down_after_write_,0,0) );}
	const SocketAddress& GetLocalAddr() const{return local_addr_;}
	const SocketAddress& GetRemoteAddr() const{return peer_addr_;}
	EventLoop* GetEventLoop(){return io_event_.GetEventLoop();}
	void SetData(void*data){data_ = data;}
	void* GetData() const{return data_;}
private:
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
	int Reader(const struct iovec *iov, int iovcnt);
	int Writer(void*buf,int size);
	void LoopTimeout();
private:
	virtual ~Connection();
	
	IOEvent io_event_;
	SocketAddress local_addr_;
	SocketAddress peer_addr_;
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

	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} 
} 

#endif /* CONNECTION_H_ */
