#include <abb/base/log.hpp>
#include <abb/base/buffer.hpp>
#include <abb/http/http_server.hpp>
#include <abb/http/http_request.hpp>
#include <abb/http/http_responce.hpp>
#include "abb/http/http_const.hpp"
class HttpHandler:public abb::http::Server::Listener{
public:
HttpHandler(){};
	virtual ~HttpHandler(){};
	virtual void HandleRequest(abb::http::Request* req,abb::http::Responce* rsp){
		abb::base::Buffer buf;
		req->Encode(buf);
		LOG(DEBUG) << std::string((char *)buf.Data(),buf.Size());
		rsp->SetStatusCode(abb::http::code::StatusOK);
		const char* hello = "hello";
		rsp->Body().Write((void*)hello,strlen(hello));
	}
};
int main(){
	abb::net::IPAddr addr;
	if( ! addr.SetV4(NULL,8080) ){
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
