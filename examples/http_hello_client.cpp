

#include <abb/base/log.hpp>
#include "abb/net/event_loop.hpp"
#include "abb/http/http_request.hpp"
#include "abb/http/http_responce.hpp"
#include "abb/http/http_const.hpp"
#include "abb/http/http_client.hpp"
#include <string.h>
using namespace abb::net;
using namespace abb;
class RequestHandler:public http::IRequestHandler{
public:
	virtual ~RequestHandler(){};
	virtual void HandleResponce(http::Responce* rsp){
		LOG(DEBUG) << std::string((char *)rsp->Body().Data(),rsp->Body().Size());
	}
	virtual void HandleError(int error){
		LOG(DEBUG) << "error:" << strerror(error);
	}
};

int main(){
	EventLoop loop;
	RequestHandler h;
	http::Get(&loop,"http:localhost:8080/",&h);
	loop.Loop();

}
