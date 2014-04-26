

#include "connection.hpp"
#include <unistd.h>
#include "socket.hpp"
#include "context.hpp"

namespace abb {
namespace net {

Connection::Connection(int fd,const IPAddr& local,const IPAddr& peer)
:poller_(ctx->GetFreePoller()),
dispatch_(ctx->GetThreadPool()),
local_addr_(local),
peer_addr_(peer),
ev_(NULL),
err_(0),
enable_(false),
em_ev_(0),
is_exe_(false),
entry_(fd,this),
fd_(fd)
{
}

Connection::~Connection() {
	poller_.DelReadWrite(&this->entry_);
	close(fd_);
}
bool Connection::Write(void*buf,int size,int* nwr){
	base::Mutex::Locker lock(wr_lock_);
	int ret = 0;
	if(this->wr_buf_.Size() != 0){
		ret = this->Writer(buf,size);
		if(ret < size && !this->err_ && this->enable_){
			this->wr_buf_.Write((char*)buf+ret,size);
			poller_.AddWrite(&this->entry_);
		}
		if(this->err_){
			if(nwr)*nwr = ret;
			if(ret > 0)return true;
			return false;
		}
	}
	if(nwr)*nwr = size;
	this->wr_buf_.Write(buf,size);
	return true;
}
bool Connection::ReadSize(void*buf,int size,int* nread){
	base::Mutex::Locker lock(rd_lock_);
	int ret = this->rd_buf_.Read(buf,size);
	if(nread)*nread = ret;
	return ret > 0;
}
void Connection::OpReadBuf(OpBuffer fn,void*arg){
	base::Mutex::Locker lock(rd_lock_);
	fn(arg,this->rd_buf_);
}
void Connection::SetEnable(bool enable){
	if(this->enable_ == enable){
		return ;
	}
	this->enable_ = enable;
	if(this->enable_){
		poller_.AddReadWrite(&this->entry_);
	}
}
int Connection::Reader(void*buf,int size){
	int nrd;
	Socket::Read(this->fd_,buf,size,&nrd,&this->err_);
	return nrd;
}
int Connection::Writer(void*buf,int size){
	int nwd;
	Socket::Write(this->fd_,buf,size,&nwd,&this->err_);
	return nwd;
}
void Connection::PollerEvent_OnRead(){
	if(this->err_){
		em_ev_ |= EVENT_ERROR;
		this->Dispatch();
		poller_.DelReadWrite(&this->entry_);
		return;
	}
	if(!this->enable_) return;
	rd_lock_.Lock();
		this->rd_buf_.WriteFromeReader(StaticReader,this);
		if(this->rd_buf_.Size() > 0){
			rd_lock_.UnLock();
			em_ev_ |= EVENT_READ;
			this->Dispatch();
		}
		if(this->err_){
			em_ev_ |= EVENT_ERROR;
			this->Dispatch();
		}
}
void Connection::PollerEvent_OnWrite(){
	if(this->err_){
		em_ev_ |= EVENT_ERROR;
		this->Dispatch();
		poller_.DelReadWrite(&this->entry_);
		return;
	}
	if(!this->enable_) return;	
	wr_lock_.Lock();
		if(this->wr_buf_.Size() > 0){
			this->wr_buf_.ReadToWriter(StaticWriter,this);
			if(this->wr_buf_.Size() == 0){
				em_ev_ |= EVENT_DRAN;
				wr_lock_.UnLock();
				this->Dispatch();
				poller_.DelWrite(&this->entry_);
			}
			wr_lock_.UnLock();
		}else{
			wr_lock_.UnLock();
			poller_.DelWrite(&this->entry_);
		}
}
void Connection::EventDispatch::Execute(){
	if(self->em_ev_&EVENT_READ){
		if(self->ev_)self->ev_->Connection_Event(EVENT_READ);
	}
	if(self->em_ev_&EVENT_DRAN){
		if(self->ev_)self->ev_->Connection_Event(EVENT_DRAN);
	}
	if(self->em_ev_&EVENT_ERROR){
		if(self->ev_)self->ev_->Connection_Event(EVENT_ERROR);
	}
	self->UnRef();
}
}}
