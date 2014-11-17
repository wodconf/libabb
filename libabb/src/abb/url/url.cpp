#include "abb/url/url.hpp"
#include "abb/base/log.hpp"
namespace abb{
namespace url{

static bool ishex(uint8_t c) {
	if( '0' <= c && c <= '9')
		return true;
	if( 'a' <= c && c <= 'f')
		return true;
	if( 'A' <= c && c <= 'F')
		return true;
	return false;
}

static uint8_t unhex(uint8_t c) {
	if('0' <= c && c <= '9')
		return c - '0';
	if('a' <= c && c <= 'f')
		return c - 'a' + 10;
	if('A' <= c && c <= 'F')
		return c - 'A' + 10;
	return 0;
}

#define encodeQueryComponent 0
#define encodePath 1
#define encodeFragment 2


static bool unescape(const std::string& s,int mode,std::string& out) {
	int n = 0;
	bool hasPlus = false;
	for(unsigned i=0;i<s.size();) {
		if(s[i] == '%'){
			n++;
			if( i+2 >= s.size() || !ishex(s[i+1]) || !ishex(s[i+2]) ) {
				std::string tmp = s.substr(i);
				if (tmp.size() > 3) {
					tmp = tmp.substr(0,3);
				}
				return false;
			}
			i += 3;
		}else if( s[i] == '+'){
			hasPlus = mode == encodeQueryComponent;
			i++;
		}else{
			i++;
		}
	}

	if( n == 0 && !hasPlus) {
		out = s;
		return true;
	}
	std::string myout;
	myout.resize(s.size() - 2*n);
	int j=0;
	for(unsigned i=0;i<s.size();) {
		if(s[i] == '%'){
			myout[j] = unhex(s[i+1])<<4 | unhex(s[i+2]);
			j++;
			i += 3;
		}else if(s[i] == '+'){
			if (mode == encodeQueryComponent) {
				myout[j] = ' ';
			} else {
				myout[j] = '+';
			}
			j++;
			i++;
		}else{
			myout[j] = s[i];
			j++;
			i++;
		}
	}
	out = myout;
	return true;
}
extern bool QueryUnescape(const std::string& str,std::string& out){
	return unescape(str,encodeQueryComponent,out);
}
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
	return unescape(left,encodePath,outurl->Path);
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
		if( !unescape(right,encodeFragment,url->Fragment) ){
			return false;
		}
	}
	if( QueryUnescape(url->RawQuery,url->RawQuery) ){
		return true;
	}
	return false;
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
