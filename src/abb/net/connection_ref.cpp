
#include "abb/net/connection_ref.hpp"
#include "abb/net/connection.hpp"
namespace abb {
namespace net {

ConnectionRef::ConnectionRef(Connection* conn)
:conn_(conn),
Data(NULL)
{
	local_ = conn_->GetLocalAddr();
	remote_ = conn_->GetRemoteAddr();
}
bool ConnectionRef::Send(void*data,int len){
	if(conn_->IsConnected()){
		conn_->SendData(data,len);
		return true;
	}
	return false;
}
bool ConnectionRef::IsClosed(){
	return !conn_->IsConnected();
}
void ConnectionRef::SetNoDelay(bool e){
	conn_->SetNoDelay(e);
}
bool  ConnectionRef::LockWrite(base::Buffer**buf){
	return conn_->LockWrite(buf);
}
void  ConnectionRef::UnLockWrite(){
	conn_->UnLockWrite();
}
void ConnectionRef::Close(){
	conn_->ShutDown(true,true);
}
void ConnectionRef::CloseAfterWrite(){
	conn_->ShutDownAfterWrite();
}

ConnectionRef::~ConnectionRef() {
	conn_->Destroy();
}
EventLoop* ConnectionRef::GetEventLoop(){
	return conn_->GetEventLoop();
}

} /* namespace net */
} /* namespace abb */
