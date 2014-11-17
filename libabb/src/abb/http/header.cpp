#include "abb/http/header.hpp"

namespace abb{
namespace http{

void Header::Set(const std::string& key,const std::string& val){
	kv_[key] = val;
}
bool Header::Get(const std::string& key,std::string& val){
	KV::iterator iter = kv_.find(key);
	if( iter!= kv_.end()){
		val = iter->second;
		return true;
	}
	return false;
}
bool Header::Delete(const std::string& key){
	KV::iterator iter = kv_.find(key);
	if( iter!= kv_.end()){
		kv_.erase(iter);
		return true;
	}
	return false;
}

}}
