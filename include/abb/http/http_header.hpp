#ifndef __ABB_HTTP_HEADER_HPP__
#define __ABB_HTTP_HEADER_HPP__

#include <map>
#include <string>

namespace abb{
namespace http{

class HeaderKey{
public:
	static const char* ACCEPT;
	static const char* ACCEPT_CHARSET ;
	static const char* ACCEPT_ENCODING; 
	static const char* ACCEPT_LANGUAGE;
	static const char* ACCEPT_RANGES;
	static const char* ACCEPT_PATCH ;
	static const char* ACCESS_CONTROL_ALLOW_CREDENTIALS;
	static const char* ACCESS_CONTROL_ALLOW_HEADERS;
	static const char* ACCESS_CONTROL_ALLOW_METHODS;
	static const char* ACCESS_CONTROL_ALLOW_ORIGIN;
	static const char* ACCESS_CONTROL_EXPOSE_HEADERS;
	static const char* ACCESS_CONTROL_MAX_AGE;
	static const char* ACCESS_CONTROL_REQUEST_HEADERS;
	static const char* ACCESS_CONTROL_REQUEST_METHOD;
	static const char* AGE;
	static const char* ALLOW;
	static const char* AUTHORIZATION;
	static const char* CACHE_CONTROL;
	static const char* CONNECTION;
	static const char* CONTENT_BASE;
	static const char* CONTENT_ENCODING;
	static const char* CONTENT_LANGUAGE;
	static const char* CONTENT_LENGTH;

	static const char* CONTENT_LOCATION;
	static const char* CONTENT_TRANSFER_ENCODING;
	static const char* CONTENT_MD5;
	static const char* CONTENT_RANGE;

	static const char* CONTENT_TYPE;
	static const char* COOKIE;
	static const char* DATE;
	static const char* ETAG;
	static const char* EXPECT;
	static const char* EXPIRES;
	static const char* FROM;
	static const char* HOST;
	static const char* IF_MATCH;
	static const char* IF_MODIFIED_SINCE;
	static const char* IF_NONE_MATCH;
	static const char* IF_RANGE;
	static const char* IF_UNMODIFIED_SINCE;
	static const char* LAST_MODIFIED;

	static const char* LOCATION;
	static const char* MAX_FORWARDS;
	static const char* ORIGIN;
	static const char* PRAGMA ;
	static const char* PROXY_AUTHENTICATE;
	static const char* PROXY_AUTHORIZATION;
	static const char* RANGE;
	static const char* REFERER;
	static const char* RETRY_AFTER;
	static const char* SEC_WEBSOCKET_KEY1;
	static const char* SEC_WEBSOCKET_KEY2 ;
	static const char* SEC_WEBSOCKET_LOCATION ;
	static const char* SEC_WEBSOCKET_ORIGIN;
	static const char* SEC_WEBSOCKET_PROTOCOL;
	static const char* SEC_WEBSOCKET_VERSION;
	static const char* SEC_WEBSOCKET_KEY;
	static const char* SEC_WEBSOCKET_ACCEPT;
	static const char* SEC_WEBSOCKET_EXTENSIONS;
	static const char* SERVER;
	static const char* SET_COOKIE;
	static const char* SET_COOKIE2;
	static const char* TE;
	static const char* TRAILER;
	static const char* TRANSFER_ENCODING;
	static const char* UPGRADE;
	static const char* USER_AGENT;
	static const char* VARY;
	static const char* VIA;
	static const char* WARNING;
	static const char* WEBSOCKET_LOCATION;
	static const char* WEBSOCKET_ORIGIN;
	static const char* WEBSOCKET_PROTOCOL ;
	static const char* WWW_AUTHENTICATE;
};
class HeaderValue{
public:
	static const char* APPLICATION_X_WWW_FORM_URLENCODED;
	static const char* BASE64;
	static const char* BINARY;
	static const char* BOUNDARY;
	static const char* BYTES ;
	static const char* CHARSET ;
	static const char* CHUNKED ;
	static const char* CLOSE ;
	static const char* COMPRESS ;
	static const char* CONTINUE ;
	static const char* DEFLATE ;
	static const char* GZIP ;
	static const char* IDENTITY;
	static const char* KEEP_ALIVE;
	static const char* MAX_AGE;
	static const char* MAX_STALE;
	static const char* MIN_FRESH;
	static const char* MULTIPART_FORM_DATA;
	static const char* MUST_REVALIDATE ;
	static const char* NO_CACHE ;
	static const char* NO_STORE;
	static const char* NO_TRANSFORM;
	static const char* NONE;
	static const char* ONLY_IF_CACHED;
	static const char* PRIVATE;
	static const char* PROXY_REVALIDATE;
	static const char* PUBLIC ;
	static const char* QUOTED_PRINTABLE ;
	static const char* S_MAXAGE ;
	static const char* TRAILERS ;
	static const char* UPGRADE ;
	static const char* WEBSOCKET ;	
};
class Header{
public:
	typedef std::map<std::string,std::string> KV;
	void Set(const std::string& key,const std::string& val);
	bool Get(const std::string& key,std::string& val);
	bool Delete(const std::string& key);
	bool Has(const std::string& key){ return kv_.find(key) != kv_.end();};
	const KV& GetKV(){return kv_;}
private:
	KV kv_;
};

}}

#endif
