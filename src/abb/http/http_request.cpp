#include "abb/http/http_request.hpp"
#include <sstream>
#define SPACE " "
#define CRLF "\r\n"
namespace abb{
namespace http{
	Request::Request( const std::string& m,const std::string& version,const std::string& url )
	:method_(m),
	 proto_(version)
	 {
		url::ParseUrl(url,&this->url_);
		this->host_ = this->url_.Host;
	}
	Request::~Request(){

	}
	bool Request::Encode(abb::base::Buffer& buf){
		std::ostringstream out;
		out << this->method_ << SPACE << this->url_.RequestURI() << SPACE << proto_ << CRLF;
		out << "Host: " << this->host_<< CRLF;
		const Header::KV& kv = this->header_.GetKV();
		Header::KV::const_iterator iter = kv.begin();
		for(;iter != kv.end();iter++){
			out << iter->first << ": " << iter->second << CRLF;
		}
		out<<CRLF;
		buf.Write((void*)out.str().c_str(),out.str().size());
		return true;
	}
}
}
