#ifndef __ABB_HTTP_REQUEST_HPP__
#define __ABB_HTTP_REQUEST_HPP__

#include <string>
#include <abb/base/buffer.hpp>
#include <abb/url/url.hpp>
#include <abb/http/http_header.hpp>
#include "abb/base/ref_object.hpp"
namespace abb{
namespace http{
	class Request:public base::RefObject{
	public:
		Request(const std::string& Method,const std::string& version);
		virtual ~Request();
		bool SetUrl(const std::string& url);
		const std::string& Method(){return method_;}
		const std::string& Protocol(){return proto_;}
		Header& GetHeader(){return header_;}
		url::URL& GetURL(){return url_;}
		bool Encode(base::Buffer& buf);
		base::Buffer& Body(){return body_;}
	private:
		std::string method_;
		std::string proto_; // "HTTP/1.0"
		Header header_;
		base::Buffer body_;
		url::URL url_;
		ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Request);
	};
}
}
#endif
