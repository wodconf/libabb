#include "abb/http/http_request.hpp"
#include <sstream>
#define SPACE " "
#define CRLF "\r\n"
namespace abb{
namespace http{
	Request::Request( const std::string& m,const std::string& version )
	:method_(m),
	 proto_(version),
	 content_buf_(NULL),
	 content_length_(0)
	 {
		
	}
	Request::~Request(){

	}
	bool Request::SetUrl(const std::string& url){
		if( url::ParseUrl(url,&this->url_) ){
			return true;
		}
		return false;
	}
	bool Request::Encode(abb::base::Buffer& buf){
		std::ostringstream out;
		out << this->method_ << SPACE << this->url_.RequestURI() << SPACE << proto_ << CRLF;
		out << "Host: " << this->url_.Host<< CRLF;
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
