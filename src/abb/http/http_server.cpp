#include "abb/http/http_server.hpp"
#include "abb/http/http_decoder.hpp"
#include "abb/http/http_responce_writer.hpp"

#include <string.h>

namespace abb{
namespace http{
void Server::L_TcpServer_OnConnection(net::ConnectionRef*ref){
	ref->Data = new RequestDecoder();
}
void Server::L_TcpServer_OnMesssage(net::ConnectionRef* ref,base::Buffer& buf){
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	if(!d->Decode(buf)){
		ref->Close();
	}else{
		Request* req = d->GetRequest();
		if(req){
			ResponceWriter* rspw = new ResponceWriter(ref,req->Protocol());
			this->lis_->HandleRequest(req,rspw);
			rspw->UnRef();
			req->UnRef();
		}
	}
}
void Server::L_TcpServer_OnClose(net::ConnectionRef* ref,int error){
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	delete d;
}

Server::Server():lis_(NULL){
	tcp_svr_.SetListener(this);
}
Server::~Server(){
	
}

}}
