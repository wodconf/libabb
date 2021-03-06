
#include "abb/net/connection_ref.hpp"
#include "abb/net/connection.hpp"
namespace abb {
namespace net {

ConnectionRef::ConnectionRef(Connection* conn)
:Data(NULL),
conn_(conn){
}
ConnectionRef::~ConnectionRef() {
	conn_->Destroy();
}
const SocketAddress& ConnectionRef::GetLocalAddr() const{
	return conn_->GetLocalAddr();
}
const SocketAddress& ConnectionRef::GetRemoteAddr() const{
	return conn_->GetRemoteAddr();
}
void ConnectionRef::Write(const void*data,int len){
	conn_->Write(data,len);
}
void ConnectionRef::Flush(){
	conn_->Flush();
}
void ConnectionRef::WriteAndFlush(const void*data,int len){
	conn_->WriteAndFlush(data,len);
}
bool ConnectionRef::Connected() const{
	return const_cast<Connection*>(conn_)->Connected();
}
bool ConnectionRef::SetNoDelay(bool e,int* error){
	return conn_->SetNoDelay(e,error);
}
bool ConnectionRef::SetKeepAlive(bool enable,int delay,int* error){
	return conn_->SetKeepAlive(enable,delay,error);
}
bool  ConnectionRef::LockWrite(Buffer**buf){
	return conn_->LockWrite(buf);
}
void  ConnectionRef::UnLockWrite(bool bflush){
	conn_->UnLockWrite(bflush);
}
void ConnectionRef::Close(){
	conn_->Close();
}


EventLoop* ConnectionRef::GetEventLoop(){
	return conn_->GetEventLoop();
}

} /* namespace net */
} /* namespace abb */
