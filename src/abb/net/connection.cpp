

#include "connection.hpp"
#include <unistd.h>
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "./poller.hpp"
#include "abb/net/event_loop.hpp"
#include <fcntl.h>
namespace abb {
namespace net {

Connection::Connection(EventLoop* loop,int fd,const IPAddr& local,const IPAddr& peer)
:loop_(loop),
 local_addr_(local),
 peer_addr_(peer),
 lis_(NULL),
 err_(0),
 fd_(fd),
 state_(STATE_OPEN),
 data_(NULL),
 io_event_(fd,this),
 enable_(false),
 shut_down_after_write_(false),
 block_write_(false)
{
	wr_buf_ = &wr_buf_1_;
	wring_buf_ = NULL;
	Socket::SetNoBlock(fd_,true);
}
bool Connection::SetKeepAlive(bool kp,int idle,int interval,int cout){
	Socket::SetKeepAlive(fd_,kp,idle,interval,cout);
}
void Connection::SetNoDelay(bool e){
	Socket::SetNoDelay(fd_,e);
}
Connection::~Connection() {
	close(fd_);
}
void Connection::Destroy(){
	SetEnable(false);
	loop_->QueueInLoop(StaticFree,this);
}
void  Connection::SetEnable(bool enable){
	if(enable_ == enable) return;
	if(enable && (state_ != STATE_OPEN)) return;
	enable_ = enable;
	if(enable_){
		io_event_.SetRead(true);
		io_event_.SetWrite(true);
		loop_->ApplyIOEvent(&io_event_);
	}else{
		io_event_.SetRead(false);
		io_event_.SetWrite(false);
		loop_->ApplyIOEvent(&io_event_);
	}
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

void Connection::UnLockWrite(){
	if(!block_write_) return;
	if(this->IsConnected()){
		Flush();
	}else{
		wr_buf_->Clear();
	}
	wr_lock_.UnLock();
}

void Connection::Flush(){
	if(this->wr_buf_->Size() > 0 && this->IsConnected()){
		io_event_.SetWrite(true);
		loop_->ApplyIOEvent(&io_event_);
	}
}

void Connection::SendData(void*buf,unsigned int size){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	wr_buf_->Write(buf,size);
	Flush();
}
int Connection::Reader(const struct iovec *iov, int iovcnt){
	int nrd;
	int err = 0;
	if( Socket::ReadV(this->fd_,iov,iovcnt,&nrd,&err) ){
		if(nrd == 0 ){
			if(err == 0){
				state_ = STATE_CLOSE;
			}
		}
	}else{
		this->err_ = err;
		state_ = STATE_ERROR;
	}
	return nrd;
}
int Connection::Writer(void*buf,int size){
	int nwd;
	Socket::Write(this->fd_,buf,size,&nwd,NULL);
	return nwd;
}
void Connection::ShutDownAfterWrite(){
	shutdown(this->fd_,SHUT_RD);
	__sync_bool_compare_and_swap((int*)&shut_down_after_write_,false,true);
	LOG(INFO) << "ShutDownAfterWrite" << shut_down_after_write_;
	io_event_.SetWrite(true);
	loop_->ApplyIOEvent(&io_event_);
}
void Connection::HandleEvent(int event){
	if(!this->enable_){
		return;
	}
	if(event&IO_EVENT_READ){
		OnRead();
	}
	if(!this->enable_){
		return;
	}
	if(event&IO_EVENT_WRITE){
		OnWrite();
	}
}
void Connection::OnRead(){
	int size = this->rd_buf_.WriteFromeReader(StaticReader,this);
	if(size > 0){
		this->lis_->L_Connection_OnMessage(this,this->rd_buf_);
	}
	if(state_ != STATE_OPEN){
		this->SetEnable(false);
		this->lis_->L_Connection_OnClose(this,this->err_);
	}
}
void Connection::OnWrite(){
	if(state_ != STATE_OPEN){
		return;
	}
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
		io_event_.SetWrite(false);
		loop_->ApplyIOEvent(&io_event_);
		if( __sync_bool_compare_and_swap((int*)&shut_down_after_write_,true,true) ){
			LOG(INFO) << "shutdown";
			this->ShutDown();
		}

	}
}
}}
