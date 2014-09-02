

#include "abb/net/connection.hpp"
#include "abb/base/log.hpp"
#include "abb/net/event_loop.hpp"
#include "abb/net/socket.hpp"
#include <errno.h>

namespace abb {
namespace net {

Connection::Connection(EventLoop* loop,int fd,const SocketAddress& local,const SocketAddress& peer)
:io_event_(loop,fd,this),
 local_addr_(local),
 peer_addr_(peer),
 lis_(NULL),
 err_(0),
 close_(0),
 data_(NULL),
 shut_down_after_write_(0),
 block_write_(false)
{
	wr_buf_ = &wr_buf_1_;
	wring_buf_ = NULL;
	Socket::SetNoBlock(io_event_.Fd(),true,NULL);
}
void Connection::Established(){
	this->GetEventLoop()->RunInLoop(StaticEstablished,this);
}
bool Connection::SetNoDelay(bool e,int* error){
	return Socket::SetNoDelay(io_event_.Fd(),e,error);
}
bool Connection::SetKeepAlive(bool enable,int delay,int* error){
	return Socket::SetKeepAlive(io_event_.Fd(),enable,delay,error);
}
void Connection::LoopedAllowWrite(){
	if( __sync_bool_compare_and_swap(&this->close_,0,0) && !io_event_.IsAllowWrite() ){
		io_event_.AllowWrite();
	}
}
Connection::~Connection() {
	if( __sync_bool_compare_and_swap(&this->close_,0,1) ){
		LOG(WARN) << "delete.before.close";
		io_event_.DisAllowAll();
		Socket::ShutDown(this->io_event_.Fd(),true,true,NULL);
	}
	Socket::Close(io_event_.Fd());
}
void Connection::LoopedEstablished(){
	io_event_.AllowRead();
	this->lis_->L_Connection_OnOpen(this);
}
void Connection::Destroy(){
	this->GetEventLoop()->QueueInLoop(StaticFree,this);
}
bool Connection::LockWrite(Buffer**buf){
	wr_lock_.Lock();
	if(this->Connected()){
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
	if(!this->Connected()){
		wr_buf_->Clear();
	}else if(bflush){
		InternalFlush();
	}
	wr_lock_.UnLock();
}
void Connection::InternalFlush(){
	if(this->wr_buf_->ReadSize() > 0 && this->Connected()){
		this->GetEventLoop()->RunInLoop(StaticAllowWrite,this);
	}
}
void Connection::Flush(){
	Mutex::Locker lock(wr_lock_);
	InternalFlush();
}
void Connection::WriteAndFlush(void*buf,unsigned int size){
	if(!this->Connected()){
		return;
	}
	Mutex::Locker lock(wr_lock_);
	wr_buf_->Write(buf,size);
	InternalFlush();
}
void Connection::Write(void*buf,unsigned int size){
	if(!this->Connected()){
		return;
	}
	Mutex::Locker lock(wr_lock_);
	wr_buf_->Write(buf,size);
}
int Connection::Reader(const struct iovec *iov, int iovcnt){
	int nrd;
	int err = 0;
	if(iov[0].iov_len == 0 ){
		LOG(WARN) <<"reader buf is empty";
		return 0;
	}
	if( Socket::ReadV(this->io_event_.Fd(),iov,iovcnt,&nrd,&err) ){
		if(nrd == 0 ){
			__sync_bool_compare_and_swap(&this->close_,0,1);
		}
	}else{
		if(err != EAGAIN){
			this->err_ = err;
			__sync_bool_compare_and_swap(&this->close_,0,1);
		}
	}
	return nrd;
}
int Connection::Writer(void*buf,int size){
	int nwd;
	int err = 0;
	if(! Socket::Write(this->io_event_.Fd(),buf,size,&nwd,&err) ){
		if(err != EAGAIN){
			this->err_ = err;
		}
	}
	return nwd;
}
void Connection::Close(){
	if(__sync_bool_compare_and_swap(&this->close_,0,0)){
		if( __sync_bool_compare_and_swap(&this->shut_down_after_write_,0,1) ){
			this->GetEventLoop()->RunInLoop(StaticAllowWrite,this);
		}
	}
}
void Connection::HandleEvent(int event){
	if(__sync_bool_compare_and_swap(&this->close_,1,1)){
		io_event_.DisAllowAll();
		return;
	}
	if(event&IO_EVENT_READ || event&IO_EVENT_ERROR){
		OnRead();
	}
	if(__sync_bool_compare_and_swap(&this->close_,1,1)) return;
	if(event&IO_EVENT_WRITE){
		OnWrite();
	}
}
void Connection::OnRead(){
	int size = this->rd_buf_.WriteFromeReader(StaticReader,this);
	if(size > 0){
		this->lis_->L_Connection_OnMessage(this,this->rd_buf_);
	}
	if(__sync_bool_compare_and_swap(&this->close_,1,1)){
		io_event_.DisAllowAll();
		Socket::ShutDown(this->io_event_.Fd(),true,true,NULL);
		this->lis_->L_Connection_OnClose(this,this->err_);
	}
}
void Connection::OnWrite(){
	if(wring_buf_){
		wring_buf_->ReadToWriter(StaticWriter,this);
		if(wring_buf_->ReadSize() == 0){
			wring_buf_ = NULL;
		}else{
			return;
		}
	}
	{
		Mutex::Locker lock(wr_lock_);
		if(wr_buf_->ReadSize() > 0){
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
		if(wring_buf_->ReadSize() == 0){
			wring_buf_ = NULL;
		}else{
			return;
		}
	}else{
		if(__sync_bool_compare_and_swap(&this->shut_down_after_write_,1,1)){
			Socket::ShutDown(this->io_event_.Fd(),true,true,NULL);
			if(__sync_bool_compare_and_swap(&this->close_,0,1)){
				io_event_.DisAllowAll();
				this->lis_->L_Connection_OnClose(this,0);
			}
		}else{
			io_event_.DisAllowWrite();
		}

	}
}


}}
