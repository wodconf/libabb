
#include "abb/net/connection_ref.hpp"
#include "abb/net/connection.hpp"
namespace abb {
namespace net {

ConnectionRef::ConnectionRef(Connection* conn)
:Data(NULL),
conn_(conn)
{
	local_ = conn_->GetLocalAddr();
	remote_ = conn_->GetRemoteAddr();
}
void ConnectionRef::Write(void*data,int len){
	conn_->Write(data,len);
}
void ConnectionRef::Flush(){
	conn_->Flush();
}
void ConnectionRef::WriteAndFlush(void*data,int len){
	conn_->WriteAndFlush(data,len);
}
bool ConnectionRef::IsClosed(){
	return !conn_->IsConnected();
}
void ConnectionRef::SetNoDelay(bool e){
	conn_->SetNoDelay(e);
}
bool  ConnectionRef::LockWrite(Buffer**buf){
	return conn_->LockWrite(buf);
}
void  ConnectionRef::UnLockWrite(bool bflush){
	conn_->UnLockWrite(bflush);
}
void ConnectionRef::Close(){
	conn_->CloseAfterWrite();
}
void ConnectionRef::CloseAfterWrite(){
	conn_->CloseAfterWrite();
}

ConnectionRef::~ConnectionRef() {
	conn_->Destroy();
}
EventLoop* ConnectionRef::GetEventLoop(){
	return conn_->GetEventLoop();
}

} /* namespace net */
} /* namespace abb */
