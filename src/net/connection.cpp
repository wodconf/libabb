

#include "connection.hpp"
#include <unistd.h>
#include "abb/net/socket.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "./poller.hpp"
#include "loop.hpp"
namespace abb {
namespace net {

Connection::Connection(Loop* loop,int fd,const IPAddr& local,const IPAddr& peer,int id)
:loop_(loop),
 local_addr_(local),
 peer_addr_(peer),
 lis_(NULL),
 err_(0),
 fd_(fd),
 bfreed_(false),
 state_(STATE_OPEN),
 id_(id),
 data_(NULL),
 entry_(fd,this),
 enable_(false)
{
	Socket::SetNoBlock(fd_,true);
}

Connection::~Connection() {
	close(fd_);
}
void Connection::Destroy(){
	base::Mutex::Locker lock(wr_lock_);
	if(bfreed_) return;
	bfreed_ = true;
	this->SetEnable(false);

	loop_->RunInLoop(StaticFree,this);
}
void  Connection::SetEnable(bool enable){
	if(state_ != STATE_OPEN) return;
	if(enable_ == enable) return;
	enable_ = enable;
	if(enable_){
		loop_->GetPoller().AddReadWrite(&this->entry_);
	}else{
		loop_->GetPoller().DelReadWrite(&this->entry_);
	}
}
base::Buffer& Connection::LockWrite(){
	wr_lock_.Lock();
	return wr_buf_;
}
void Connection::Flush(){
	this->wr_buf_.ReadToWriter(StaticWriter,this);
	if(this->wr_buf_.Size() > 0 && this->IsConnected()){
		loop_->GetPoller().AddWrite(&this->entry_);
	}
}
void Connection::UnLockWrite(){
	if(this->IsConnected()){
		Flush();
	}else{
		wr_buf_.Clear();
	}
	wr_lock_.UnLock();
}
void Connection::SendData(void*buf,unsigned int size){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	this->wr_buf_.Write(buf,size);
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
void Connection::PollerEvent_OnRead(){
	if(this->bfreed_){
		return;
	}
	if(!this->enable_){
		return;
	}
	int size = this->rd_buf_.WriteFromeReader(StaticReader,this);
	if(size > 0){
		this->lis_->L_Connection_OnMessage(this,this->rd_buf_);
	}
	if(state_ != STATE_OPEN){
		this->SetEnable(false);
		this->lis_->L_Connection_OnClose(this,this->err_);
	}
}
void Connection::PollerEvent_OnWrite(){
	if(state_ != STATE_OPEN){
		return;
	}
	if(this->bfreed_){
		return;
	}
	if(!this->enable_){
		return;
	}
	wr_lock_.Lock();
	if(this->wr_buf_.Size() > 0){
		this->wr_buf_.ReadToWriter(StaticWriter,this);
		int size = this->wr_buf_.Size();
		wr_lock_.UnLock();
		if( size == 0){
			loop_->GetPoller().DelWrite(&this->entry_);
		}
	}else{
		wr_lock_.UnLock();
		loop_->GetPoller().DelWrite(&this->entry_);
	}
}
}}
