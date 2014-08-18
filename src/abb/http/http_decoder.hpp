#ifndef __ABB_HTTP_REQUEST_DECODER_HPP__
#define __ABB_HTTP_REQUEST_DECODER_HPP__

#include "abb/http/http_request.hpp"
#include "abb/http/http_responce.hpp"
#include "abb/base/buffer.hpp"
#include "abb/base/log.hpp"
namespace abb{
namespace http{	
	class RequestDecoder{
	public:
		RequestDecoder();
		~RequestDecoder();
		bool Decode(Buffer& buf);
		Request* GetRequest(){
			if(state_ == STATE_COMPLETE){
				return req_;
			}
			return NULL;
		}
		int State(){return state_;}
	private:
		enum{
			FIRST_HEAD,
			STATE_HEAD,
			STATE_BODY,
			STATE_COMPLETE,
		};
		Request* req_;
		int state_;
		int len_;
	};
	class ResponceDecoder{
	public:
		ResponceDecoder();
		~ResponceDecoder();
		bool Decode(Buffer& buf);
		Responce* GetResponce(){
			if(state_ == STATE_COMPLETE){
				return rsp_;
			}
			return NULL;
		}
		int State(){return state_;}
	private:
		enum{
			FIRST_HEAD,
			STATE_HEAD,
			STATE_BODY,
			STATE_COMPLETE,
		};
		Responce* rsp_;
		int state_;
		int len_;
	};
}
}



#endif

