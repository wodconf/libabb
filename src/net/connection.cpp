

#include "abb/net/connection.hpp"
#include <unistd.h>
#include "socket.hpp"
#include "context.hpp"
#include "abb/base/log.hpp"
namespace abb {
namespace net {

Connection::Connection(Context* ctx,int fd,const IPAddr& local,const IPAddr& peer)
:loop_(ctx->GetFreeLoop()),
 local_addr_(local),
 peer_addr_(peer),
 ev_(NULL),
 err_(0),
 enable_(false),
 is_exe_(false),
 entry_(fd,this),
 fd_(fd),
 bfreed_(false),
 ctx_(ctx),
 dis(this)
{
	Socket::SetNoBlock(fd_,true);
}

Connection::~Connection() {
	close(fd_);
}
void Connection::Destroy(){
	if(bfreed_) return;
	bfreed_ = true;
	enable_ = false;
	loop_.GetPoller().DelReadWrite(&this->entry_);
	loop_.RunInLoop(StaticFree,this);
}
bool Connection::Write(void*buf,int size,int* nwr){
	base::Mutex::Locker lock(wr_lock_);
	if(this->err_ != 0){
		return false;
	}
	int ret = 0;
	if(this->wr_buf_.Size() == 0){
		ret = this->Writer(buf,size);
		if(ret < size && !this->err_){
			this->wr_buf_.Write((char*)buf+ret,size);
			if(this->enable_)
				loop_.GetPoller().AddWrite(&this->entry_);
		}
		if(this->err_){
			if(nwr)*nwr = ret;
			if(ret > 0)return true;
			return false;
		}
		return true;
	}else{
		if(nwr)*nwr = size;
		this->wr_buf_.Write(buf,size);
		return true;
	}

}
bool Connection::ReadSize(void*buf,int size,int* nread){
	base::Mutex::Locker lock(rd_lock_);
	int ret = this->rd_buf_.Read(buf,size);
	if(nread)*nread = ret;
	return ret > 0;
}
base::Buffer& Connection::LockWrite(){
	wr_lock_.Lock();
	return this->wr_buf_;
}
void Connection::UnLockWrite(){
	this->wr_buf_.ReadToWriter(StaticWriter,this);
	if(this->wr_buf_.Size() != 0){
		loop_.GetPoller().AddWrite(&this->entry_);;
	}
	wr_lock_.UnLock();
}
base::Buffer& Connection::LockRead(){
	rd_lock_.Lock();
	return this->rd_buf_;
}
void Connection::UnLockRead(){
	rd_lock_.UnLock();
}

void Connection::SetEnable(bool enable){
	if(this->enable_ == enable){
		return ;
	}
	this->enable_ = enable;
	if(this->enable_){
		loop_.GetPoller().AddReadWrite(&this->entry_);
	}
}
int Connection::Reader(void*buf,int size){
	int nrd;
	Socket::Read(this->fd_,buf,size,&nrd,&this->err_);
	if(this->err_){
		LOG(INFO) << "Connection::Reader ERROR:" << err_ << strerror(err_);
	}
	return nrd;
}
int Connection::Writer(void*buf,int size){
	int nwd;
	Socket::Write(this->fd_,buf,size,&nwd,&this->err_);
	if(this->err_){
		LOG(INFO) << "Connection::Writer ERROR:" << err_ << strerror(err_);
	}
	LOG(INFO) << "Connection::Writer WRITED:" << nwd;
	return nwd;
}
void Connection::PollerEvent_OnRead(){
	LOG(INFO) << "PollerEvent_OnRead";
	if(this->err_ != 0){
		loop_.GetPoller().DelReadWrite(&this->entry_);
		this->Dispatch();
		return;
	}
	LOG(INFO) << "PollerEvent_OnRead 1";
	if(!this->enable_) return;
	LOG(INFO) << "PollerEvent_OnRead 2";
	rd_lock_.Lock();
	LOG(INFO) << "PollerEvent_OnRead 3";
	this->rd_buf_.WriteFromeReader(StaticReader,this);
	int size = this->rd_buf_.Size();
	LOG(INFO) << "size:" << size;
	rd_lock_.UnLock();
	if(size >  0 || this->err_){
		this->Dispatch();
	}
}
void Connection::PollerEvent_OnWrite(){
	if(this->err_ != 0){
		this->Dispatch();
		loop_.GetPoller().DelReadWrite(&this->entry_);
		return;
	}
	if(!this->enable_) return;
	wr_lock_.Lock();
	if(this->wr_buf_.Size() > 0){
		this->wr_buf_.ReadToWriter(StaticWriter,this);
		int size = this->wr_buf_.Size();
		wr_lock_.UnLock();
		if( size == 0){
			loop_.GetPoller().DelWrite(&this->entry_);
		}
	}else{
		wr_lock_.UnLock();
		loop_.GetPoller().DelWrite(&this->entry_);
	}
}
void Connection::Dispatch(){
	if(!is_exe_){
		is_exe_ = true;
		this->Ref();
		this->ctx_->GetThreadPool().Execute(&dis);
	}
}
void Connection::DoEmmit(){
	while(!this->bfreed_ && this->ev_){
		this->rd_lock_.Lock();
		int sz = this->rd_buf_.Size();
		this->rd_lock_.UnLock();
		if( sz > 0){
			this->ev_->Connection_Event(EVENT_READ);
		}else{
			if( this->err_ != 0 ){
				this->ev_->Connection_Event(EVENT_ERROR);
			}
			break;
		}
	}
	this->is_exe_ = false;
	this->UnRef();
}

void Connection::EventDispatch::Execute(){
	self->DoEmmit();
}
}}
