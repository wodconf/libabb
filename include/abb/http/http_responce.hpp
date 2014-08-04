#ifndef __ABB_NET_HTTP_RESPONCE_HPP__
#define __ABB_NET_HTTP_RESPONCE_HPP__

#include <string>
#include <abb/base/buffer.hpp>
#include <abb/url/url.hpp>
#include <abb/http/http_header.hpp>
#include "abb/base/ref_object.hpp"

namespace abb{
namespace http{

class Responce{
public:
	Responce(const std::string& version);
	~Responce();
	void SetStatusCode(int code){
		code_ = code;
	}
	int GetStatusCode(){return code_;}
	const std::string& Protocol(){return proto_;}
	Header& GetHeader(){return header_;}
	bool Encode(base::Buffer& buf);
	base::Buffer& Body(){return body;}
private:
	std::string proto_;
	Header header_;
	int code_;
	base::Buffer body;
};

}
}

#endif
