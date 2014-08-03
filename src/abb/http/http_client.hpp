#ifndef __ABB_HTTP_CLIENT_HPP__
#define __ABB_HTTP_CLIENT_HPP__

#include <string>

#include "abb/http/http_request.hpp"

namespace abb{
namespace http{

class IRequestHandler{
public:
	virtual ~IRequestHandler();
	virtual void HandleResponce(Responce* rsp);
	virtual void HandleError(int error);
};

extern bool POST(EventLoop* loop,
				std::string& url,
				const std::string& body_type,
				void* body,
				int size,
				IRequestHandler* handler);
extern bool GET(EventLoop* loop,std::string& url,IRequestHandler* handler);
extern void Do(EventLoop* loop,Request* req,IRequestHandler* handler);

}}
#endif