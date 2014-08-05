#include "abb/http/http_server.hpp"
#include "abb/http/http_decoder.hpp"
#include "abb/http/http_responce_writer.hpp"
namespace abb{
namespace http{
void Server::L_TcpServer_OnConnection(net::ConnectionRef*req){
	req->Data = new RequestDecoder();
}
void Server::L_TcpServer_OnMesssage(net::ConnectionRef* ref,base::Buffer& buf){
	LOG(DEBUG) << "L_TcpServer_OnMesssage" << std::string((char*)buf.Data(),buf.Size());
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	if(!d->Decode(buf)){
		LOG(WARN) << "close";
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
	LOG(DEBUG) << "L_TcpServer_OnClose";
	RequestDecoder* d = static_cast<RequestDecoder*>(ref->Data);
	delete d;
}

Server::Server():lis_(NULL){
	tcp_svr_.SetListener(this);
}
Server::~Server(){
	
}

}}
