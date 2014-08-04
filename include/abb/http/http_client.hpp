#ifndef __ABB_HTTP_CLIENT_HPP__
#define __ABB_HTTP_CLIENT_HPP__

#include <string>

#include "abb/http/http_request.hpp"

namespace abb{
namespace net{
class EventLoop;
}
namespace http{

class Responce;
class IRequestHandler{
public:
	virtual ~IRequestHandler(){};
	virtual void HandleResponce(Responce* rsp) = 0;
	virtual void HandleError(int error) = 0;
};

extern bool Post(net::EventLoop* loop,
				std::string& url,
				const std::string& body_type,
				void* body,
				int size,
				IRequestHandler* handler);
extern bool Get(net::EventLoop* loop,std::string& url,IRequestHandler* handler);
extern bool Do(net::EventLoop* loop,Request* req,IRequestHandler* handler);

}}
#endif
