#include "http_header.hpp"

namespace abb{
namespace http{

const char* HeaderKey::ACCEPT = "Accept";
const char* HeaderKey::ACCEPT_CHARSET = "Accept-Charset";
const char* HeaderKey::ACCEPT_ENCODING = "Accept-Encoding";
const char* HeaderKey::ACCEPT_LANGUAGE = "Accept-Language";
const char* HeaderKey::ACCEPT_RANGES = "Accept-Ranges";
const char* HeaderKey::ACCEPT_PATCH = "Accept-Patch";
const char* HeaderKey::ACCESS_CONTROL_ALLOW_CREDENTIALS = "Access-Control-Allow-Credentials";
const char* HeaderKey::ACCESS_CONTROL_ALLOW_HEADERS = "Access-Control-Allow-Headers";
const char* HeaderKey::ACCESS_CONTROL_ALLOW_METHODS = "Access-Control-Allow-Methods";
const char* HeaderKey::ACCESS_CONTROL_ALLOW_ORIGIN = "Access-Control-Allow-Origin";
const char* HeaderKey::ACCESS_CONTROL_EXPOSE_HEADERS = "Access-Control-Expose-Headers";
const char* HeaderKey::ACCESS_CONTROL_MAX_AGE = "Access-Control-Max-Age";
const char* HeaderKey::ACCESS_CONTROL_REQUEST_HEADERS = "Access-Control-Request-Headers";
const char* HeaderKey::ACCESS_CONTROL_REQUEST_METHOD = "Access-Control-Request-Methods";
const char* HeaderKey::AGE = "Age";
const char* HeaderKey::ALLOW = "Allow";
const char* HeaderKey::AUTHORIZATION = "Authorization";
const char* HeaderKey::CACHE_CONTROL = "Cache-Control";
const char* HeaderKey::CONNECTION = "Connection";

const char* HeaderKey::CONTENT_BASE = "Content-Base";
const char* HeaderKey::CONTENT_ENCODING = "Content-Encoding";
const char* HeaderKey::CONTENT_LANGUAGE = "Content-Language";
const char* HeaderKey::CONTENT_LENGTH = "Content-Length";
const char* HeaderKey::CONTENT_LOCATION = "Content-Location";
const char* HeaderKey::CONTENT_TRANSFER_ENCODING = "Content-Transfer-Encoding";
const char* HeaderKey::CONTENT_MD5 = "Content-MD5";
const char* HeaderKey::CONTENT_RANGE = "Content-Range";
const char* HeaderKey::CONTENT_TYPE = "Content-Type";

const char* HeaderKey::COOKIE = "Cookie";
const char* HeaderKey::DATE = "Date";
const char* HeaderKey::ETAG = "ETag";
const char* HeaderKey::EXPECT = "Expect";
const char* HeaderKey::EXPIRES = "Expires";
const char* HeaderKey::FROM = "From";
const char* HeaderKey::HOST = "Host";
const char* HeaderKey::IF_MATCH = "If-Match";
const char* HeaderKey::IF_MODIFIED_SINCE = "If-Modified-Since";
const char* HeaderKey::IF_NONE_MATCH = "If-None-Match";
const char* HeaderKey::IF_RANGE = "If-Range";
const char* HeaderKey::IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
const char* HeaderKey::LAST_MODIFIED = "Last-Modified";

const char* HeaderKey::LOCATION = "Location";
const char* HeaderKey::MAX_FORWARDS = "Max-Forwards";
const char* HeaderKey::ORIGIN = "Origin";
const char* HeaderKey::PRAGMA = "Pragma" ;
const char* HeaderKey::PROXY_AUTHENTICATE = "Proxy-Authenticate";
const char* HeaderKey::PROXY_AUTHORIZATION = "Proxy-Authorization";
const char* HeaderKey::RANGE = "Range";
const char* HeaderKey::REFERER = "Referer";
const char* HeaderKey::RETRY_AFTER = "Retry-After";
const char* HeaderKey::SEC_WEBSOCKET_KEY1 = "Sec-WebSocket-Key1";
const char* HeaderKey::SEC_WEBSOCKET_KEY2 = "Sec-WebSocket-Key2";
const char* HeaderKey::SEC_WEBSOCKET_LOCATION = "Sec-WebSocket-Location";
const char* HeaderKey::SEC_WEBSOCKET_ORIGIN = "Sec-WebSocket-Origin";
const char* HeaderKey::SEC_WEBSOCKET_PROTOCOL = "Sec-WebSocket-Protocol";
const char* HeaderKey::SEC_WEBSOCKET_VERSION = "Sec-WebSocket-Version";
const char* HeaderKey::SEC_WEBSOCKET_KEY = "Sec-WebSocket-Key";
const char* HeaderKey::SEC_WEBSOCKET_ACCEPT = "Sec-WebSocket-Accept";
const char* HeaderKey::SEC_WEBSOCKET_EXTENSIONS = "Sec-WebSocket-Extensions";
const char* HeaderKey::SERVER = "Server";
const char* HeaderKey::SET_COOKIE = "Set-Cookie";
const char* HeaderKey::SET_COOKIE2 = "Set-Cookie2";
const char* HeaderKey::TE = "TE";
const char* HeaderKey::TRAILER = "Trailer";
const char* HeaderKey::TRANSFER_ENCODING = "Transfer-Encoding";
const char* HeaderKey::UPGRADE = "Upgrade";
const char* HeaderKey::USER_AGENT = "User-Agent";
const char* HeaderKey::VARY = "Vary";
const char* HeaderKey::VIA = "Via";
const char* HeaderKey::WARNING = "Warning";
const char* HeaderKey::WEBSOCKET_LOCATION = "WebSocket-Location";
const char* HeaderKey::WEBSOCKET_ORIGIN = "WebSocket-Origin";
const char* HeaderKey::WEBSOCKET_PROTOCOL = "WebSocket-Protocol";
const char* HeaderKey::WWW_AUTHENTICATE = "WWW-Authenticate";


const char* HeaderValue::APPLICATION_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";


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
