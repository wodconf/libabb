#include "abb/http/http_server.hpp"
#include "abb/http/http_decoder.hpp"
#include "abb/http/http_responce.hpp"
namespace abb{
namespace http{
void Server::L_TcpServer_OnConnection(net::ConnectionRef*req){
	req->Data = new RequestDecoder();
}
void Server::L_TcpServer_OnMesssage(net::ConnectionRef* ref,base::Buffer& buf){
	LOG(DEBUG) << "L_TcpServer_OnMesssage";
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	if(!d->Decode(buf)){
		ref->Close();
	}else{
		Request* req = d->GetRequest();
		if(req){
			Responce rsp(req->Protocol());
			this->lis_->HandleRequest(req,&rsp);
			abb::base::Buffer* buf;
			if( ref->LockWrite(&buf)){
				rsp.Encode(*buf);
				ref->UnLockWrite();
			}
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
