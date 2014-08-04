#ifndef __ABB_HTTP_SESSION_HPP__
#define __ABB_HTTP_SESSION_HPP__

#include "abb/http/http_request.hpp"

namespace abb{
namespace http{	
	class RequestDecoder{
	public:
		RequestDecoder();
		~RequestDecoder();
		bool Decode(base::Buffer& buf);
		Request* GetRequest(){
			if(state_ == STATE_COMPLETE){
				return req_;
			}
		}
	private:
		enum{
			FIRST_HEAD,
			STATE_HEAD,
			STATE_BODY,
			STATE_COMPLETE,
		};
		int state_;
		Request* req_;
		int len_;
	};
}
}



#endif