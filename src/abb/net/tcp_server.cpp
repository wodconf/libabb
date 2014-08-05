#include "abb/net/tcp_server.hpp"
#include "abb/net/event_loop_group.hpp"
#include "abb/net/connection.hpp"
#include "abb/net/acceptor.hpp"
#include "abb/net/connection_ref.hpp"
#include "abb/net/event_loop.hpp"
namespace abb{
namespace net{
TcpServer::TcpServer():lis_(NULL),acceptor_(NULL),event_group_(NULL),loop_(NULL){

}
TcpServer::~TcpServer(){

}
void TcpServer::Init(int num_thread,bool run_curent_thread){
	event_group_ = new EventLoopGroup(num_thread);

	if(run_curent_thread){
		loop_ = new EventLoop();
		acceptor_ = new Acceptor(loop_);
	}else{
		acceptor_ = new Acceptor(event_group_->Next());
	}
}
bool TcpServer::Bind(const IPAddr& addr,int* save_error){
	return acceptor_->Bind(addr,save_error);
}
const IPAddr& TcpServer::GetAddr(){
	return acceptor_->GetIpAddr();
}
void TcpServer::Start(){
	acceptor_->SetListener(this);
	acceptor_->SetEnable(true);
	event_group_->Start();
	if(loop_){
		loop_->Loop();
	}
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
	this->lis_->L_TcpServer_OnClose(ref,error);
	ref->UnRef();
}
}
}
