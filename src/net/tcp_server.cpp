#include "abb/net/tcp_server.hpp"
#include "context.hpp"
#include "connection.hpp"
#include "acceptor.hpp"
#include "abb/net/connection_ref.hpp"
namespace abb{
namespace net{
TcpServer::TcpServer():lis_(NULL),acceptor_(NULL),id_(1),ctx_(NULL){

}
TcpServer::~TcpServer(){

}
void TcpServer::Init(int num_thread,bool run_curent_thread){
	ctx_ = new Context();
	ctx_->SetNumThread(num_thread);
	ctx_->SetRunCureentThread(run_curent_thread);
	ctx_->Init();
	acceptor_ = new Acceptor(ctx_->GetFreeLoop());
}
bool TcpServer::Bind(const IPAddr& addr,int* save_error){
	return acceptor_->Listen(addr,save_error);
}
void TcpServer::Start(){
	ctx_->Start();
	acceptor_->SetListener(this);
	acceptor_->SetEnable(true);
}
void TcpServer::L_Acceptor_OnConnection(Acceptor* ptr,int fd,const IPAddr& addr){
	int id = id_++;
	Connection* conn = new Connection(ctx_->GetFreeLoop(),fd,acceptor_->GetIpAddr(),addr,id);
	ConnectionRef* ref = new ConnectionRef(conn);
	conn->SetData(ref);
	ref->Ref();
	{
		base::RWLock::WLocker lock(rwlock_);
		this->conn_map_[id] = conn;
	}
	conn->SetListener(this);
	conn->SetEnable(true);
	this->lis_->L_TcpServer_OnConnection(ref);
}
void TcpServer::L_Connection_OnMessage(Connection* conn,base::Buffer& buf){
	this->lis_->L_TcpServer_OnMesssage((ConnectionRef*)conn->GetData(),buf);
}
void TcpServer::L_Connection_OnClose(Connection* conn,int error){
	ConnectionRef* ref = (ConnectionRef*)conn->GetData();
	int id = conn->GetId();
	conn->ShutDown();
	conn->Destroy();
	{
		base::RWLock::WLocker lock(rwlock_);
		this->conn_map_.erase(id);
	}
	if(ref->TestAndSetNULL()){
		this->lis_->L_TcpServer_OnClose(ref,error);
	}
	ref->UnRef();
}
}
}
