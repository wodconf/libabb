#include "abb/http/http_request.hpp"
#include <sstream>
#define SPACE " "
#define CRLF "\r\n"
namespace abb{
namespace http{
	Request::Request( const std::string& Method,const std::string& version ){

	}
	Request::~Request(){

	}
	bool Request::Encode(abb::base::Buffer& buf){
		std::ostringstream out;
		out << this->Method << SPACE << this->url_->RequestURI() << SPACE << proto_ << CRLF;
		out << "Host: " << this->host_<< CRLF;
		const Header::KV& kv = this->header_.GetKv();
		Header::KV::const_iterator iter = kv.begin();
		for(;iter != kv.end();iter++){
			out << iter->first << ": " << iter->second << CRLF;
		}
		return true;
	}
}
}
