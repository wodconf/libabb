#include "abb/url/url.hpp"
#include "abb/base/log.hpp"
namespace abb{
namespace url{
static void split(const std::string& s, const std::string& c,bool cutc,std::string& left,std::string& rigth ) {
	std::string::size_type pos = s.find(c);
	if(pos == std::string::npos){
		left = s;
		rigth = "";
		return ;
	}
	if (cutc) {
		left = s.substr(0,pos);
		rigth = s.substr(pos+c.size());
		return;
	}
	left = s.substr(0,pos);
	rigth = s.substr(pos);
} 
static bool getscheme(const std::string& rawurl ,std::string& scheme,std::string& path);
static bool parse(const std::string& url,URL* outurl,bool viaRequest){
	if(url == "" && viaRequest){
		LOG(WARN) << "empty url";
		return false;
	}

	if(url == "*"){
		outurl->Path = "*";
		return true;
	}
	//http://test.com/path?q=1
	std::string rest;
	if(!getscheme(url,outurl->Scheme,rest)){
		return false;
	}
	//  //test.com/path?q=1
	std::string left;//  
	split(rest, "?", true, left , outurl->RawQuery);
	// //test.com/path
	if(left.find("/") != 0){
		if(outurl->Scheme != ""){
			outurl->Opaque = left;
			return true;
		}else if(viaRequest){
			LOG(WARN) << "invalid URI for request";
			return false;
		}
	}

	if(outurl->Scheme != "" || !viaRequest && left.find("///") != 0 && left.find("//") == 0){
	// test.com/path
		split(left.substr(2), "/", false,outurl->Host,left);
		//  /path
		if( outurl->Host.find("%") != std::string::npos){
			LOG(WARN) << "hexadecimal escape in host";
			return false;
		}
	}
	outurl->Path = left;
	return true;
}
static bool getscheme(const std::string& rawurl ,std::string& scheme,std::string& path) {
	for(unsigned i=0;i<rawurl.size();i++) {
		char c = rawurl[i];
		if( ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ){
			;
		}else if( ('0' <= c && c <= '9') || c == '+' || c == '-' || c == '.'){
			if( i == 0 ) {
				scheme = "";
				path = rawurl; 
			}
		}else if(c == ':'){
			if(i == 0){
				scheme = "";
				path = "";
				//miss scheme;
				return false;
			}else{
				scheme = rawurl.substr(0,i);
				path = rawurl.substr(i+1);
				return true;
			}
		}else{
			scheme = "";
			path = rawurl; 
			return true;
		}
	}
	scheme = "";
	path = rawurl; 
	return true;
}
bool ParseUrl(const std::string& inurl,URL* url){
	std::string left,right;
	split(inurl,"#",false,left,right);

	if( !parse(left,url,false) ){
		return false;
	}
	if( !right.empty() ){
		url->Fragment = right;
	}
	return true;
}
std::string URL::RequestURI(){
	std::string result = this->Opaque;
	if (result == "") {
		result = this->Path;
		if (result == "") {
			result = "/";
		}
	} else {
		if(result.find("//") == 0){
			result = this->Scheme + ":" + result;
		}
	}
	if (this->RawQuery != "") {
		result += "?" + this->RawQuery;
	}
	return result;
}
}}
