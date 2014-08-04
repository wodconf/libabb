#include "abb/http/http_server.hpp"
#include "abb/http/http_request_decoder.hpp"
namespace abb{
namespace http{
void Server::L_TcpServer_OnConnection(ConnectionRef*req){
	req->Data = new RequestDecoder();
}
void Server::L_TcpServer_OnMesssage(ConnectionRef* ref,base::Buffer& buf){
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	if(!d->Decode(buf)){
		ref->Close();
	}else{
		Request* req = d->GetRequest();
		if(req){
			this->lis_.OnRequest(req);
		}
	}
}
void Server::L_TcpServer_OnClose(ConnectionRef* ref,int error){
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	delete d;
}

Server::Server():lis_(NULL){
	tcp_svr_.SetListener(this);
}
Server::~Server(){
	
}

}}