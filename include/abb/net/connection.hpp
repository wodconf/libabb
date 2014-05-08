

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
#include "poller.hpp"
#include <sys/socket.h>
#include "i_protocol.hpp"
namespace abb {
namespace net {
class Loop;
class Context;
class Connection :public IPollerEvent,private base::RefObject{
public:
	static Connection* Create(Context* ctx,int fd,const IPAddr& local,const IPAddr& peer){
		return new Connection(ctx,fd,local,peer);
	}
private:
	Connection(Context* ctx,int fd,const IPAddr& local,const IPAddr& peer);
	virtual ~Connection();
public:
	class IEvent{
	public:
		virtual ~IEvent(){};
		virtual void Connection_OnMessage(Connection* con,Msg msg);
		virtual void Connection_OnClose(Connection* con);
	};
	void SetEventCallBack(IEvent* event){ev_ = event;}
	void Send(Msg msg);
	void SendData(void*buf,unsigned int size);
	int ShutDown(int how){
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
	friend struct EventDispatch;
	IPAddr local_addr_;
	IPAddr peer_addr_;
	bool is_exe_;
	EventDispatch dis;
	int fd_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer wr_buf_;
	Context* ctx_;
	Loop& loop_;
	Poller::Entry entry_;
	bool enable_;
	int err_;
	enum{
		STATE_OPEN,
		STATE_CLOSE,
		STATE_ERROR
	}state_;
	IEvent* ev_;
	int bfreed_;
	IProtocol* protocol_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
