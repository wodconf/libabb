#include "abb/net/tcp_server.hpp"
#include "./context.hpp"
namespace abb{
namespace net{
TcpServer::TcpServer(const IPAddr& addr);
TcpServer::~TcpServer();
void TcpServer::SetNumThread(int num){
	ctx_.SetNumThread(num);
}
void TcpServer::SetListener(Listener* lis){lis_ = lis;}
bool TcpServer::Bind(int* save_error){

}
void TcpServer::Start(bool run_cur_thread){

}
void TcpServer::L_Connector_OnOpen(Connection* cotr,Connection* conn){

}
void TcpServer::L_Connector_OnClose(Connection* cotr){

}
void TcpServer::L_Acceptor_OnConnection(Acceptor* ptr,Connection* conn){

}
}
}