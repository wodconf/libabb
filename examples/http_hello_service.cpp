#include <abb/base/log.hpp>
#include <abb/base/buffer.hpp>
#include <abb/http/http_server.hpp>
#include <abb/http/http_request.hpp>

class HttpHandler:public abb::http::Server::Listener{
	HttpHandler(){};
	virtual ~HttpHandler(){};
	virtual void OnRequest(Request* req){
		abb::base::Buffer buf;
		req.Encode(buf);
		LOG(DEBUG) << std::string((char *)buf.Data(),buf.Size());
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
		exit(0);
	}
	HttpHandler h;
	svr.SetListener(&h);
	svr.Start();
}
