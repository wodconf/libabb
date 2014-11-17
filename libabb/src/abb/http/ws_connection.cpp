
#include "abb/http/ws_connection.hpp"
#include "abb/http/websocket.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/base/log.hpp"

namespace abb{
namespace http{

WSConnection::WSConnection(net::ConnectionRef* ref,WebSocket* pro):Data(NULL),ws_(pro),conn_ref_(ref),lis_(NULL){
	conn_ref_->Ref();
}
WSConnection::~WSConnection(){
	conn_ref_->UnRef();
	delete ws_;
}
void WSConnection::SendText(const std::string& text ,bool flush){
	abb::Buffer* buf;
	if( conn_ref_->LockWrite(&buf)){
		ws_->MakeFrame(TEXT_FRAME,text.c_str(),text.size(),*buf);
		conn_ref_->UnLockWrite(flush);
	}
}

void WSConnection::SendBinary(const void* data,int size ,bool flush){
	abb::Buffer* buf;
	if( conn_ref_->LockWrite(&buf)){
		ws_->MakeFrame(BINARY_FRAME,data,size,*buf);
		conn_ref_->UnLockWrite(flush);
	}
}
void WSConnection::Close(){
	conn_ref_->Close();
}
void WSConnection::OnMessage( Buffer& buf ){
	uint64_t len;
	WebSocketFrameType frame = ws_->GetFrame(buf,&len);
	if(frame == ERROR_FRAME){
		if(this->lis_)this->lis_->OnClose(this,0);
	}else if(frame == INCOMPLETE_FRAME){
		//
	}else{
		if(frame == TEXT_FRAME || frame == INCOMPLETE_TEXT_FRAME){
			std::string str((const char*)buf.ReadPtr(),len);
			buf.GaveRead(len);
			if(this->lis_)this->lis_->OnTextMessage(this,str);
		}else if(frame == BINARY_FRAME || frame == INCOMPLETE_BINARY_FRAME){
			if(this->lis_)this->lis_->OnBinaryMessage(this,buf.ReadPtr(),len);
			buf.GaveRead(len);
		}else if(frame == CLOSE_FRAME){
			this->Close();
		}else if(frame == PING_FRAME){
			buf.GaveRead(len);
		}else if(frame == PONG_FRAME){
			buf.GaveRead(len);
		}else{
			//
			this->Close();
		}
	} 
}
void WSConnection::OnClose( int error ){
	if(this->lis_){
		this->lis_->OnClose(this,error);
	}
}
const std::string& WSConnection::GetOrigin() const {
	return ws_->origin;
}
const std::string& WSConnection::GetPath()  const {
	return ws_->path;
}
const std::string& WSConnection::GetQuery() const {
	return ws_->query;
}
const std::string& WSConnection::GetProtocol() const{
	return ws_->protocol;
}
net::EventLoop* WSConnection::GetEventLoop(){
	return conn_ref_->GetEventLoop();
}
const net::SocketAddress& WSConnection::GetLocalAddr() const{
	return conn_ref_->GetLocalAddr();
}
const net::SocketAddress& WSConnection::GetRemoteAddr() const{
	return conn_ref_->GetRemoteAddr();
}

bool WSConnection::Connected() const {
	return conn_ref_->Connected();
}
void WSConnection::Flush(){
	conn_ref_->Flush();
}
}}
