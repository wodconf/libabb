

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
 entry_(fd,this),
 fd_(fd),
 bfreed_(false),
 ctx_(ctx),
 state_(STATE_OPEN)
{
	Socket::SetNoBlock(fd_,true);
	loop_.GetPoller().AddReadWrite(&this->entry_);
	protocol_ = this->ctx_->GetProtocol();
}

Connection::~Connection() {
	close(fd_);
	this->ctx_->BackProtocol(protocol_);
}
void Connection::Destroy(){
	base::Mutex::Locker lock(wr_lock_);
	if(bfreed_) return;
	bfreed_ = true;
	loop_.GetPoller().DelReadWrite(&this->entry_);
	loop_.RunInLoop(StaticFree,this);
}
void Connection::SendMsg(void* msg){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	this->protocol_->Encode(this->wr_buf_,msg);
	this->wr_buf_.ReadToWriter(StaticWriter,this);
	if(this->wr_buf_.Size() != 0){
		loop_.GetPoller().AddWrite(&this->entry_);
	}
}
void Connection::SendData(void*buf,unsigned int size){
	if(!this->IsConnected()){
		return;
	}
	base::Mutex::Locker lock(wr_lock_);
	this->wr_buf_.Write(buf,size);
	this->wr_buf_.ReadToWriter(StaticWriter,this);
	if(this->wr_buf_.Size() != 0){
		loop_.GetPoller().AddWrite(&this->entry_);
	}
}
int Connection::Reader(const struct iovec *iov, int iovcnt){
	int nrd;
	if( Socket::ReadV(this->fd_,iov,iovcnt,&nrd,&this->err_) ){
		if(nrd == 0 ){
			if(err_ == 0){
				state_ = STATE_CLOSE;
			}
		}
	}else{
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
	this->rd_buf_.WriteFromeReader(StaticReader,this);
	this->Ref();
	while( this->rd_buf_.Size()){
		if(this->ev_){
			Msg msg = this->protocol_->Decode(rd_buf_);
			this->ev_->Connection_OnMessage(this,msg);
		}else{
			break;
		}
	}
	if(state_ != STATE_OPEN){
		loop_.GetPoller().DelReadWrite(&this->entry_);
		if(this->ev_)
			this->ev_->Connection_OnClose(this);
	}
	this->UnRef();
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
			loop_.GetPoller().DelWrite(&this->entry_);
		}
	}else{
		wr_lock_.UnLock();
		loop_.GetPoller().DelWrite(&this->entry_);
	}
}
}}
