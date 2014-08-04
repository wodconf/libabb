#include "abb/http/http_request.hpp"
#include "abb/http/http_const.hpp"
#include <sstream>

namespace abb{
namespace http{
	Request::Request( const std::string& m,const std::string& version )
	:method_(m),
	 proto_(version)
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
		out << this->method_ << " " << this->url_.RequestURI() << " " << proto_ << "\r\n";
		out << "Host: " << this->url_.Host<< "\r\n";
		const Header::KV& kv = this->header_.GetKV();
		Header::KV::const_iterator iter = kv.begin();
		for(;iter != kv.end();iter++){
			out << iter->first << ": " << iter->second << "\r\n";
		}
		if(body.Size() > 0){
			out << head::CONTENT_LENGTH << ": "<< body.Size() << "\r\n";
		}
		out<<"\r\n";
		buf.Write((void*)out.str().c_str(),out.str().size());
		buf.Write(body.Data(),body.Size());
		return true;
	}
}
}
