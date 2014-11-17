#include "abb/http/server.hpp"
#include "abb/http/decoder.hpp"
#include "abb/http/responce_writer.hpp"
#include "abb/http/ws_connection.hpp"
#include <string.h>
#include "websocket.hpp"

namespace abb{
namespace http{
struct Context{
	Context():ws_(NULL){
		decoder_ = new RequestDecoder();
	}
	~Context(){
		if(ws_)ws_->UnRef();
		delete decoder_;
	}
	RequestDecoder* decoder_;
	WSConnection* ws_;
};
void Server::L_TcpServer_OnConnection(net::ConnectionRef*ref){
	ref->Data = new Context();
	ref->SetNoDelay(true,NULL);
}
void Server::L_TcpServer_OnMesssage(net::ConnectionRef* ref,Buffer& buf){
	Context* ctx = static_cast<Context*>(ref->Data);
	if(ctx->ws_){
		LOG(DEBUG) << "OnMessage";
		ctx->ws_->OnMessage(buf);
		return;
	}
	if(!ctx->decoder_->Decode(buf)){
		ref->Close();
	}else{
		Request* req = ctx->decoder_->GetRequest();
		if(req){
			if(req->Method() == "GET"){
				std::string val;
				if( req->GetHeader().Get("Connection",val) && val.find("Upgrade") != std::string::npos){
					if( req->GetHeader().Get("Upgrade",val) && val == "websocket"){

						WebSocket* pro = new WebSocket();
						req->GetHeader().Get("Host",pro->host);
						req->GetHeader().Get("Origin",pro->origin);
						req->GetHeader().Get("Sec-WebSocket-Key",pro->key);
						req->GetHeader().Get("Sec-WebSocket-Protocol",pro->protocol);
						req->GetHeader().Get("Sec-WebSocket-Version",pro->version);
						std::string ret = pro->answerHandshake();
						pro->path = req->GetURL().Path;
						pro->query = req->GetURL().RawQuery;
						LOG(DEBUG) << ret << "--------------";
						ref->WriteAndFlush(ret.c_str(),ret.size());
						ctx->ws_ = new WSConnection(ref,pro);
						this->lis_->HandleWebSocket(ctx->ws_);
						return;
					}else{
						LOG(DEBUG) << "FAIL : " << val;
						ref->Close();
					}
					return;
				}
			}
			ResponceWriter* rspw = new ResponceWriter(ref,req->Protocol());
			this->lis_->HandleRequest(req,rspw);
			rspw->UnRef();
			req->UnRef();
		}
	}
}
void Server::L_TcpServer_OnClose(net::ConnectionRef* ref,int error){
	Context* ctx = static_cast<Context*>(ref->Data);
	if(ctx->ws_){
		ctx->ws_->OnClose(error);
	}
	delete ctx;
}

Server::Server():lis_(NULL){
	tcp_svr_.SetListener(this);
}
Server::~Server(){
	
}

}}
