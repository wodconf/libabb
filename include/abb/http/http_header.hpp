#ifndef __ABB_HTTP_HEADER_HPP__
#define __ABB_HTTP_HEADER_HPP__

#include <map>
#include <string>

#include <abb/base/define.hpp>

namespace abb{
namespace http{

class Header{
public:
	Header(){}
	~Header(){}
	typedef std::map<std::string,std::string> KV;
	void Set(const std::string& key,const std::string& val);
	bool Get(const std::string& key,std::string& val);
	bool Delete(const std::string& key);
	bool Has(const std::string& key){ return kv_.find(key) != kv_.end();};
	const KV& GetKV(){return kv_;}
private:
	KV kv_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Header);
};

}}

#endif
