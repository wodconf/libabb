#ifndef __ABB_NET_HTTP_RESPONCE_HPP__
#define __ABB_NET_HTTP_RESPONCE_HPP__

#include <string>
#include <abb/base/buffer.hpp>
#include <abb/url/url.hpp>
#include <abb/http/header.hpp>
#include "abb/base/ref_object.hpp"

namespace abb{
namespace http{

class Responce:public RefObject{
public:
	Responce(const std::string& version);
	virtual ~Responce();
	void SetStatusCode(const int code){
		code_ = code;
	}
	int GetStatusCode(){return code_;}
	const std::string& Protocol(){return proto_;}
	Header& GetHeader(){return header_;}
	bool Encode(Buffer& buf);
	Buffer& Body(){return body_;}
private:
	std::string proto_;
	Header header_;
	int code_;
	Buffer body_;
};

}
}

#endif
