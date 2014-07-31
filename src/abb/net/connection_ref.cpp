
#include "abb/net/connection_ref.hpp"
#include "abb/net/connection.hpp"
namespace abb {
namespace net {

ConnectionRef::ConnectionRef(Connection* conn,AcceptorRef* parent)
:conn_(conn),
Data(NULL),
parent_(parent)
{
	local_ = conn_->GetLocalAddr();
	remote_ = conn_->GetRemoteAddr();
	parent_->Ref();
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
void ConnectionRef::SetKeepAlive(bool kp,int idle,int interval,int cout){
	conn_->SetKeepAlive(kp,idle,interval,cout);
}
bool  ConnectionRef::LockWrite(base::Buffer**buf){
	return conn_->LockWrite(buf);
}
void  ConnectionRef::UnLockWrite(){
	conn_->UnLockWrite();
}
void ConnectionRef::Close(){
	conn_->ShutDown();
}
void ConnectionRef::CloseAfterWrite(){
	conn_->ShutDownAfterWrite();
}

ConnectionRef::~ConnectionRef() {
	conn_->Destroy();
	parent_->UnRef();
}


} /* namespace net */
} /* namespace abb */
