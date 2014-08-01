#ifndef __ABB_HTTP_SESSION_HPP__
#define __ABB_HTTP_SESSION_HPP__

#include <string>
#include <abb/base/buffer.hpp>
#include <abb/url/url.hpp>
#include "abb/base/ref_object.hpp"
namespace abb{
namespace{
	class ConnectionRef;
}
namespace http{
	class Request:public base::RefObject{
	public:
		Request(const std::string& Method,const std::string& version);
		~Request();
		const std::string& Method(){return method_;}
		const std::string& Protocol(){return proto_;}
		const std::string& ContentLength(){return content_length_;}
		Header& Header(){return header_;}
		url::URL& URL(){return url_};
		bool Encode(abb::base::Buffer& buf);
	private:
		std::string method_;
		std::string proto_; // "HTTP/1.0"
		Header header_;
		void* content_buf_;
		uint32_t content_length_;
		std::string host_;
		url::URL url_;
	};
}
}
#endif