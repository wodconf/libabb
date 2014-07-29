#include "abb/net/tcp_server.hpp"
#include "abb/net/event_loop_group.hpp"
#include "abb/net/connection.hpp"
#include "abb/net/acceptor.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb{
namespace net{
TcpServer::TcpServer():lis_(NULL),acceptor_(NULL),event_group_(NULL){

}
TcpServer::~TcpServer(){

}
void TcpServer::Init(int num_thread,bool run_curent_thread){
	event_group_ = new EventLoopGroup(num_thread);
	acceptor_ = new Acceptor(event_group_->Next());
}
bool TcpServer::Bind(const IPAddr& addr,int* save_error){
	return acceptor_->Listen(addr,save_error);
}
const IPAddr& TcpServer::GetAddr(){
	return acceptor_->GetIpAddr();
}
void TcpServer::Start(){
	acceptor_->SetListener(this);
	acceptor_->SetEnable(true);
	event_group_->Start();
}
void TcpServer::Close(){
	acceptor_->Close();
}
void TcpServer::Pause(){
	acceptor_->SetEnable(false);
}
void TcpServer::Resume(){
	acceptor_->SetEnable(true);
}
void TcpServer::L_Acceptor_OnConnection(Acceptor* ptr,int fd,const IPAddr& addr){
	Connection* conn = new Connection(event_group_->Next(),fd,acceptor_->GetIpAddr(),addr);
	ConnectionRef* ref = new ConnectionRef(conn);
	conn->SetData(ref);
	ref->Ref();
	/*{
		base::Mutex::Locker lock(mtx_);
		this->conn_map_[long(conn)] = conn;
	}*/
	conn->SetListener(this);
	conn->SetEnable(true);
	this->lis_->L_TcpServer_OnConnection(ref);
}
void TcpServer::L_Connection_OnMessage(Connection* conn,base::Buffer& buf){
	this->lis_->L_TcpServer_OnMesssage((ConnectionRef*)conn->GetData(),buf);
}
void TcpServer::L_Connection_OnClose(Connection* conn,int error){
	ConnectionRef* ref = (ConnectionRef*)conn->GetData();
	conn->ShutDown();
	conn->Destroy();
	/*{
		base::Mutex::Locker lock(mtx_);
		this->conn_map_.erase(long(conn));
	}*/
	ref->TestAndSetNULL();
	this->lis_->L_TcpServer_OnClose(ref,error);
	ref->UnRef();
}
}
}
