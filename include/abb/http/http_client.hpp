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
				const std::string& url,
				const std::string& body_type,
				const void* body,
				int size,
				IRequestHandler* handler);
extern bool Get(net::EventLoop* loop,const std::string& url,IRequestHandler* handler);
extern bool Do(net::EventLoop* loop,Request* req,IRequestHandler* handler);

extern Responce* SyncDo(Request& req,int* error,int ms_timeout);
extern Responce* SyncPost(const std::string& url,
						const std::string& body_type,
						const void* body,
						int size,int* error,int ms_timeout);
extern Responce* SyncGet(const std::string& url,int* error,int ms_timeout);
}}
#endif
