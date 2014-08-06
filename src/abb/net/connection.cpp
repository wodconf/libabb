

#include "abb/net/connection.hpp"
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include "abb/net/event_loop.hpp"
#include <errno.h>

namespace abb {
namespace net {

Connection::Connection(EventLoop* loop,int fd,const IPAddr& local,const IPAddr& peer)
:io_event_(loop,fd,this),
 local_addr_(local),
 peer_addr_(peer),
 lis_(NULL),
 err_(0),
 state_(STATE_OPEN),
 data_(NULL),
 shut_down_after_write_(false),
 block_write_(false)
{
	wr_buf_ = &wr_buf_1_;
	wring_buf_ = NULL;
	Socket::SetNoBlock(io_event_.Fd(),true);
	io_event_.AllowRead();
}
void Connection::SetNoDelay(bool e){
	Socket::SetNoDelay(io_event_.Fd(),e);
}
Connection::~Connection() {
	io_event_.DisAllowAll();
	Socket::Close(io_event_.Fd());
}
void Connection::Destroy(){
	this->GetEventLoop()->QueueInLoop(StaticFree,this);
}
void Connection::ShutDown(bool read,bool write){
	Socket::ShutDown(this->io_event_.Fd(),read,write,NULL);
}
bool Connection::LockWrite(base::Buffer**buf){
	wr_lock_.Lock();
	if(this->IsConnected()){
		block_write_ = true;
		*buf = wr_buf_;
		return true;
	}else{
		wr_lock_.UnLock();
		return false;
	}
}

void Connection::UnLockWrite(bool bflush){
	if(!block_write_) return;
	if(!this->IsConnected()){
		wr_buf_->Clear();
	}else if(bflush){
		InternalFlush();
	}
	wr_lock_.UnLock();
}
void Connection::InternalFlush(){
	if(this->wr_buf_->Size() > 0 && this->IsConnected()){
		io_event_.AllowWrite();
	}
}
void Connection::Flush(){
	base::Mutex::Locker lock(wr_lock_);
	InternalFlush();
}
void Connection::WriteAndFlush(void*buf,unsigned int size){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	wr_buf_->Write(buf,size);
	InternalFlush();
}
void Connection::Write(void*buf,unsigned int size){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	wr_buf_->Write(buf,size);
}
int Connection::Reader(const struct iovec *iov, int iovcnt){
	int nrd;
	int err = 0;
	if( Socket::ReadV(this->io_event_.Fd(),iov,iovcnt,&nrd,&err) ){
		if(nrd == 0 ){
			if(err == 0){
				state_ = STATE_CLOSE;
			}
		}
	}else{
		this->err_ = err;
		state_ = STATE_CLOSE;
	}
	return nrd;
}
int Connection::Writer(void*buf,int size){
	int nwd;
	int err = 0;
	if(! Socket::Write(this->io_event_.Fd(),buf,size,&nwd,&err) ){
		this->err_ = err;
		state_ = STATE_CLOSE;
	}
	return nwd;
}
void Connection::ShutDownAfterWrite(){
	base::Mutex::Locker lock(wr_lock_);
	if(shut_down_after_write_) return;
	shut_down_after_write_ = true;
	io_event_.AllowWrite();
}
void Connection::HandleEvent(int event){
	if(event&IO_EVENT_READ){
		OnRead();
	}
	if(event&IO_EVENT_WRITE){
		OnWrite();
	}
}
void Connection::OnRead(){
	if(state_ == STATE_CLOSE) return;
	int size = this->rd_buf_.WriteFromeReader(StaticReader,this);
	if(size > 0){
		this->lis_->L_Connection_OnMessage(this,this->rd_buf_);
	}
	if(state_ == STATE_CLOSE){
		io_event_.DisAllowAll();
		this->lis_->L_Connection_OnClose(this,this->err_);
	}
}
void Connection::OnWrite(){
	if(state_ == STATE_CLOSE) return;
	if(wring_buf_){
		wring_buf_->ReadToWriter(StaticWriter,this);
		if(wring_buf_->Size() == 0){
			wring_buf_ = NULL;
		}else{
			return;
		}
	}
	{
		base::Mutex::Locker lock(wr_lock_);
		if(wr_buf_->Size() > 0){
			wring_buf_ = wr_buf_;
			if(wr_buf_ == &wr_buf_1_){
				wr_buf_ = &wr_buf_2_;
			}else{
				wr_buf_ = &wr_buf_1_;
			}
		}
	}
	if(wring_buf_){
		wring_buf_->ReadToWriter(StaticWriter,this);
		if(wring_buf_->Size() == 0){
			wring_buf_ = NULL;
		}else{
			return;
		}
	}else{
		base::Mutex::Locker lock(wr_lock_);
		io_event_.DisAllowWrite();
		if(shut_down_after_write_){
			this->ShutDown(true,true);
		}

	}
}


}}
