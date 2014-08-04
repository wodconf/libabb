#include "abb/http/http_responce.hpp"
#include "abb/http/http_const.hpp"
#include <sstream>
namespace abb{
namespace http{

Responce::Responce(const std::string& version)
:proto_(version),
code_(code::StatusOK)
{

}
Responce::~Responce(){

}
bool Responce::Encode(base::Buffer& buf){
	std::ostringstream out;
	out << proto_ << " " << code_ << "\r\n";
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