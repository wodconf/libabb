

#ifndef __ABB_HTTP_CONST_HPP__
#define __ABB_HTTP_CONST_HPP__

namespace abb{
namespace http{
namespace method{
	const char*	GET = "GET";
	const char*	POST = "POST";
	const char*	PUT = "PUT";
	const char*	DELETE = "DELETE";
}
namespace header{
	const char* ACCEPT = "Accept";
	const char* ACCEPT_CHARSET = "Accept-Charset";
	const char* ACCEPT_ENCODING = "Accept-Encoding";
	const char* ACCEPT_LANGUAGE = "Accept-Language";
	const char* ACCEPT_RANGES = "Accept-Ranges";
	const char* ACCEPT_PATCH = "Accept-Patch";
	const char* ACCESS_CONTROL_ALLOW_CREDENTIALS = "Access-Control-Allow-Credentials";
	const char* ACCESS_CONTROL_ALLOW_HEADERS = "Access-Control-Allow-Headers";
	const char* ACCESS_CONTROL_ALLOW_METHODS = "Access-Control-Allow-Methods";
	const char* ACCESS_CONTROL_ALLOW_ORIGIN = "Access-Control-Allow-Origin";
	const char* ACCESS_CONTROL_EXPOSE_HEADERS = "Access-Control-Expose-Headers";
	const char* ACCESS_CONTROL_MAX_AGE = "Access-Control-Max-Age";
	const char* ACCESS_CONTROL_REQUEST_HEADERS = "Access-Control-Request-Headers";
	const char* ACCESS_CONTROL_REQUEST_METHOD = "Access-Control-Request-Methods";
	const char* AGE = "Age";
	const char* ALLOW = "Allow";
	const char* AUTHORIZATION = "Authorization";
	const char* CACHE_CONTROL = "Cache-Control";
	const char* CONNECTION = "Connection";

	const char* CONTENT_BASE = "Content-Base";
	const char* CONTENT_ENCODING = "Content-Encoding";
	const char* CONTENT_LANGUAGE = "Content-Language";
	const char* CONTENT_LENGTH = "Content-Length";
	const char* CONTENT_LOCATION = "Content-Location";
	const char* CONTENT_TRANSFER_ENCODING = "Content-Transfer-Encoding";
	const char* CONTENT_MD5 = "Content-MD5";
	const char* CONTENT_RANGE = "Content-Range";
	const char* CONTENT_TYPE = "Content-Type";

	const char* COOKIE = "Cookie";
	const char* DATE = "Date";
	const char* ETAG = "ETag";
	const char* EXPECT = "Expect";
	const char* EXPIRES = "Expires";
	const char* FROM = "From";
	const char* HOST = "Host";
	const char* IF_MATCH = "If-Match";
	const char* IF_MODIFIED_SINCE = "If-Modified-Since";
	const char* IF_NONE_MATCH = "If-None-Match";
	const char* IF_RANGE = "If-Range";
	const char* IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
	const char* LAST_MODIFIED = "Last-Modified";

	const char* LOCATION = "Location";
	const char* MAX_FORWARDS = "Max-Forwards";
	const char* ORIGIN = "Origin";
	const char* PRAGMA = "Pragma" ;
	const char* PROXY_AUTHENTICATE = "Proxy-Authenticate";
	const char* PROXY_AUTHORIZATION = "Proxy-Authorization";
	const char* RANGE = "Range";
	const char* REFERER = "Referer";
	const char* RETRY_AFTER = "Retry-After";
	const char* SEC_WEBSOCKET_KEY1 = "Sec-WebSocket-Key1";
	const char* SEC_WEBSOCKET_KEY2 = "Sec-WebSocket-Key2";
	const char* SEC_WEBSOCKET_LOCATION = "Sec-WebSocket-Location";
	const char* SEC_WEBSOCKET_ORIGIN = "Sec-WebSocket-Origin";
	const char* SEC_WEBSOCKET_PROTOCOL = "Sec-WebSocket-Protocol";
	const char* SEC_WEBSOCKET_VERSION = "Sec-WebSocket-Version";
	const char* SEC_WEBSOCKET_KEY = "Sec-WebSocket-Key";
	const char* SEC_WEBSOCKET_ACCEPT = "Sec-WebSocket-Accept";
	const char* SEC_WEBSOCKET_EXTENSIONS = "Sec-WebSocket-Extensions";
	const char* SERVER = "Server";
	const char* SET_COOKIE = "Set-Cookie";
	const char* SET_COOKIE2 = "Set-Cookie2";
	const char* TE = "TE";
	const char* TRAILER = "Trailer";
	const char* TRANSFER_ENCODING = "Transfer-Encoding";
	const char* UPGRADE = "Upgrade";
	const char* USER_AGENT = "User-Agent";
	const char* VARY = "Vary";
	const char* VIA = "Via";
	const char* WARNING = "Warning";
	const char* WEBSOCKET_LOCATION = "WebSocket-Location";
	const char* WEBSOCKET_ORIGIN = "WebSocket-Origin";
	const char* WEBSOCKET_PROTOCOL = "WebSocket-Protocol";
	const char* WWW_AUTHENTICATE = "WWW-Authenticate";


	const char* APPLICATION_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";
	const char* BASE64 = "base64";
	const char* BINARY = "binary";
	const char* BOUNDARY = "boundary";
	const char* BYTES = "bytes";
	const char* CHARSET = "charset";
	const char* CHUNKED = "chunked";
	const char* CLOSE = "close";
	const char* COMPRESS = "compress";
	const char* CONTINUE = "100-continue";
	const char* DEFLATE = "deflate";
	const char* GZIP = "gzip";
	const char* IDENTITY = "identity";
	const char* KEEP_ALIVE = "keep-alive";
	const char* MAX_AGE = "max-age";
	const char* MAX_STALE = "max-stale";
	const char* MIN_FRESH = "min-fresh";
	const char* MULTIPART_FORM_DATA = "multipart/form-data";
	const char* MUST_REVALIDATE = "must-revalidate";
	const char* NO_CACHE = "no-cache";
	const char* NO_STORE = "no-store";
	const char* NO_TRANSFORM = "no-transform";
	const char* NONE = "none";
	const char* ONLY_IF_CACHED = "only-if-cached";
	const char* PRIVATE = "private";
	const char* PROXY_REVALIDATE = "proxy-revalidate";
	const char* PUBLIC = "public";
	const char* QUOTED_PRINTABLE = "quoted-printable";
	const char* S_MAXAGE = "s-maxage";
	const char* TRAILERS = "trailers";
	const char* UPGRADE = "Upgrade";
	const char* WEBSOCKET = "WebSocket";
}


}
}

#endif
