

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
	http::Request req(http::method::GET,"HTTP/1.1");
	req.SetUrl("http://www.baidu.com/");
	req.GetHeader().Set(http::header::USER_AGENT," Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:30.0) Gecko/20100101 Firefox/30.0");
	req.GetHeader().Set(http::header::CONTENT_LENGTH,"0");
	req.GetHeader().Set(http::header::ACCEPT,"*/*");
	req.GetHeader().Set(http::header::CONNECTION,"Keep-Alive");
	req.GetHeader().Set(http::header::ACCEPT_LANGUAGE,"zh-cn");
	req.GetHeader().Set(http::header::ACCEPT_ENCODING,"gzip, deflate");
	RequestHandler h;
	if( !http::Do(&loop,&req,&h) ){
		LOG(DEBUG) << "FAIL";
	}
	http::Get(&loop,"http://www.baidu.com/",&h);
	loop.Loop();

}
