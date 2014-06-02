

#include "abb/net/connection.hpp"
#include <unistd.h>
#include "socket.hpp"
#include "context.hpp"
#include "abb/base/log.hpp"
#include <errno.h>
#include "./poller.hpp"
namespace abb {
namespace net {

Connection::Connection(Loop* loop,int fd,const IPAddr& local,const IPAddr& peer)
:loop_(loop),
 local_addr_(local),
 peer_addr_(peer),
 ev_(NULL),
 err_(0),
 entry_(fd,this),
 fd_(fd),
 bfreed_(false),
 state_(STATE_OPEN)
{
	Socket::SetNoBlock(fd_,true);
	entry_ = new PollerEntry(fd,this);
	loop_->GetPoller().AddReadWrite(this->entry_);
}

Connection::~Connection() {
	close(fd_);
}
void Connection::Destroy(){
	base::Mutex::Locker lock(wr_lock_);
	if(bfreed_) return;
	bfreed_ = true;
	loop_->GetPoller().DelReadWrite(this->entry_);
	loop_->RunInLoop(StaticFree,this);
}
base::Buffer& Connection::LockWrite(){
	wr_lock_.Lock();
	return wr_buf_;
}
void Connection::Flush(){
	this->wr_buf_.ReadToWriter(StaticWriter,this);
	if(this->wr_buf_.Size() > 0 && this->IsConnected()){
		loop_->GetPoller().AddWrite(this->entry_);
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
	int size = this->rd_buf_.WriteFromeReader(StaticReader,this);
	if(size > 0){
		this->lis_->L_Connection_EventRead(this,this->rd_buf_);
	}
	if(state_ != STATE_OPEN){
		loop_->GetPoller().DelReadWrite(this->entry_);
		this->lis_->L_Connection_EventClose(this);
	}
}
void Connection::PollerEvent_OnWrite(){
	if(state_ != STATE_OPEN){
		return;
	}
	if(this->bfreed_){
		return;
	}
	wr_lock_.Lock();
	if(this->wr_buf_.Size() > 0){
		this->wr_buf_.ReadToWriter(StaticWriter,this);
		int size = this->wr_buf_.Size();
		wr_lock_.UnLock();
		if( size == 0){
			loop_->GetPoller().DelWrite(this->entry_);
		}
	}else{
		wr_lock_.UnLock();
		loop_->GetPoller().DelWrite(this->entry_);
	}
}
}}
