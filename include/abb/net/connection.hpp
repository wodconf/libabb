

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "i_poller_event.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/base/thread_pool.hpp"
#include "abb/base/mutex.hpp"
#include "abb/base/rw_lock.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
#include "abb/net/ip_addr.hpp"
#include "poller.hpp"
#include <sys/socket.h>
namespace abb {
namespace net {
class Loop;
class Connection :public IPollerEvent,base::RefObject{
public:
	static Connection* New(int fd,const IPAddr& local,const IPAddr& peer){
		return new Connection(fd,local,peer);
	}
private:
	Connection(int fd,const IPAddr& local,const IPAddr& peer);
	virtual ~Connection();
public:
	enum{
		EVENT_READ = 0x01,
		EVENT_DRAN = 0x02,
		EVENT_ERROR = 0x04,
	};
	class IEvent{
	public:
		virtual void Connection_Event(int)=0;
	};
	typedef void(*OpBuffer)(void*arg,base::Buffer&buf);
	void SetEventCallBack(IEvent* event){ev_ = event;}
	bool Write(void*buf,int size,int* nwr);
	bool ReadSize(void*buf,int size,int* nwr);
	void OpReadBuf(OpBuffer fn,void*arg);
	int ShutDown(int how){
		return shutdown(this->fd_,how);
	}
	void SetEnable(bool enable);
	void Free();
	virtual void PollerEvent_OnRead();
	virtual void PollerEvent_OnWrite();
private:
	static int StaticReader(void*arg,void*buf,int size){
		Connection* con = (Connection*)arg;
		return con->Reader(buf,size);
	}
	static int StaticWriter(void*arg,void*buf,int size){
		Connection* con = (Connection*)arg;
		return con->Writer(buf,size);
	}
	static void StaticFree(void*arg){
		Connection* con = (Connection*)arg;
		con->UnRef();
	}
	int Reader(void*buf,int size);
	int Writer(void*buf,int size);
	void Dispatch(){
		if(is_exe_){
			is_exe_ = true;
			this->Ref();
			this->dispatch_.Execute(&dis);
		}
	}
private:
	struct EventDispatch:public base::ThreadPool::Runer{
		int ref;
		Connection* self;
		virtual ~EventDispatch(){};
		virtual void Execute();
	};
	friend struct EventDispatch;
	IPAddr local_addr_;
	IPAddr peer_addr_;
	bool is_exe_;
	EventDispatch dis;
	int fd_;
	base::Mutex rd_lock_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer wr_buf_;
	base::ThreadPool& dispatch_;
	Loop& loop_;
	Poller::Entry entry_;
	bool enable_;
	int err_;
	IEvent* ev_;
	int em_ev_;
	int bfreed_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
