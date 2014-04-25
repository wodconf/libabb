

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "poller_able.hpp"
#include "abb/base/ref_object.hpp"
#include "abb/base/thread_pool.hpp"
#include "abb/base/mutex.hpp"
#include "abb/base/rw_lock.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/define.hpp"
namespace abb {
namespace net {
class Poller;
class Connection :public PollerAble,base::RefObject{
public:
	Connection* New(int fd,Poller& p,base::ThreadPool& pool){
		return new Connection(fd,p,pool);
	}
private:
	Connection(int fd,Poller& p,base::ThreadPool& pool);
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
	bool Write(void*buf,int size,int* nread);
	void Read(void*buf,int size,int* nread);
	void OpReadBuf(OpBuffer fn,void*arg);
	int ShutDown(int how){
		return shutdown(this->GetFd(),how);
	}
	void SetEnable(bool enable);
	void Free(){
		this->UnRef();
	}
private:
	static int StaticReader(void*arg,void*buf,int size){
		Connection* con = (Connection*)arg;
		return con->Reader(buf,size);
	}
	static int StaticWriter(void*arg,void*buf,int size){
		Connection* con = (Connection*)arg;
		return con->Writer(buf,size);
	}
	int Reader(void*buf,int size);
	int Writer(void*buf,int size);
	virtual void OnEvent(int event);
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
	bool is_exe_;
	EventDispatch dis;
	int fd_;
	base::Mutex rd_lock_;
	base::Buffer rd_buf_;
	base::Mutex wr_lock_;
	base::Buffer wr_buf_;
	base::ThreadPool& dispatch_;
	bool enable_;
	int err_;
	IEvent* ev_;
	int em_ev_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Connection);
};

} /* namespace translate */
} /* namespace adcloud */

#endif /* CONNECTION_H_ */
