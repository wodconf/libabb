#include <abb/base/log.hpp>
#include <abb/base/buffer.hpp>
#include <abb/http/http_server.hpp>
#include <abb/http/http_request.hpp>
#include <abb/http/http_responce_writer.hpp>
#include "abb/http/http_const.hpp"
#include "abb/http/ws_connection.hpp"
class HttpHandler:public abb::http::Server::Listener,public abb::http::WSConnection::Listener{
public:
HttpHandler(){};
	virtual ~HttpHandler(){};
	virtual void HandleRequest(abb::http::Request* req,abb::http::ResponceWriter* rspw){
		rspw->GetResponce().SetStatusCode(abb::http::code::StatusNotFound);
		rspw->GetResponce().GetHeader().Set(abb::http::header::CONTENT_TYPE,"text/plain");
		char back[] = "StatusNotFound";
		rspw->GetResponce().Body().Write(back,strlen(back));
		rspw->Flush();
		return ;

		if(req->Body().ReadSize() > 0){
			LOG(DEBUG) << "request body ->  " <<std::string((char *)req->Body().ReadPtr(),req->Body().ReadSize());
		}
		rspw->GetResponce().SetStatusCode(abb::http::code::StatusOK);
		rspw->GetResponce().GetHeader().Set(abb::http::header::CONTENT_TYPE,"text/plain");
		rspw->GetResponce().GetHeader().Set("Access-Control-Allow-Origin","*");
		
		const char* hello = "hello";
		rspw->GetResponce().Body().Write((void*)hello,strlen(hello));
		rspw->Flush();
	}
	virtual void HandleWebSocket(abb::http::WSConnection* conn){
		conn->SetListener(this);
		conn->SendText("hello");
	};
	virtual void OnTextMessage(abb::http::WSConnection* conn,const std::string& str){
		LOG(DEBUG) << "MSG" << str;
	}
	virtual void OnBinaryMessage(abb::http::WSConnection* conn,void* buf,int size) {

	}
	virtual void OnClose(abb::http::WSConnection* conn,int error) {

	}
};
int main(){
	abb::net::SocketAddress addr;
	if( ! addr.SetV4(NULL,8010) ){
		LOG(INFO) << "setv4 fail";
		return -1;
	}
	abb::http::Server svr;
	svr.Init(4,true);
	int err;
	if( ! svr.Bind(addr,&err) ){
		LOG(INFO) << "Bind fail" << strerror(err);
		return 0;
	}
	HttpHandler h;
	svr.SetListener(&h);
	svr.Start();
}
