
#include "abb/net/connection_ref.hpp"
#include "connection.hpp"
namespace abb {
namespace net {

ConnectionRef::ConnectionRef(Connection* conn):conn_(conn),Data(NULL),block_write_(false) {
	// TODO Auto-generated constructor stub

}
bool ConnectionRef::Send(void*data,int len){
	base::Mutex::Locker l(mtx_);
	if(conn_ && conn_->IsConnected()){
		conn_->SendData(data,len);
		return true;
	}
	return false;
}
bool  ConnectionRef::LockWrite(base::Buffer**buf){
	mtx_.Lock();
	if(conn_ && conn_->IsConnected()){
		block_write_ = true;
		*buf = &conn_->LockWrite();
		return true;
	}else{
		mtx_.UnLock();
		return false;
	}
}
void  ConnectionRef::UnLockWrite(){
	if(!block_write_)return;
	block_write_ = false;
	conn_->UnLockWrite();
	mtx_.UnLock();
}
void ConnectionRef::Close(){
	base::Mutex::Locker l(mtx_);
	if(conn_){
		conn_->ShutDown();
		conn_ = NULL;
	}
}
ConnectionRef::~ConnectionRef() {

}

} /* namespace net */
} /* namespace abb */
