

#ifndef __ABB_HTTP_CONST_HPP__
#define __ABB_HTTP_CONST_HPP__

namespace abb{
namespace http{
namespace method{
	const char* const	GET = "GET";
	const char* const	POST = "POST";
	const char* const	PUT = "PUT";
	const char* const	DELETE = "DELETE";
}
namespace header{
	const char* const ACCEPT = "Accept";
	const char* const ACCEPT_CHARSET = "Accept-Charset";
	const char* const ACCEPT_ENCODING = "Accept-Encoding";
	const char* const ACCEPT_LANGUAGE = "Accept-Language";
	const char* const ACCEPT_RANGES = "Accept-Ranges";
	const char* const ACCEPT_PATCH = "Accept-Patch";
	const char* const ACCESS_CONTROL_ALLOW_CREDENTIALS = "Access-Control-Allow-Credentials";
	const char* const ACCESS_CONTROL_ALLOW_HEADERS = "Access-Control-Allow-Headers";
	const char* const ACCESS_CONTROL_ALLOW_METHODS = "Access-Control-Allow-Methods";
	const char* const ACCESS_CONTROL_ALLOW_ORIGIN = "Access-Control-Allow-Origin";
	const char* const ACCESS_CONTROL_EXPOSE_HEADERS = "Access-Control-Expose-Headers";
	const char* const ACCESS_CONTROL_MAX_AGE = "Access-Control-Max-Age";
	const char* const ACCESS_CONTROL_REQUEST_HEADERS = "Access-Control-Request-Headers";
	const char* const ACCESS_CONTROL_REQUEST_METHOD = "Access-Control-Request-Methods";
	const char* const AGE = "Age";
	const char* const ALLOW = "Allow";
	const char* const AUTHORIZATION = "Authorization";
	const char* const CACHE_CONTROL = "Cache-Control";
	const char* const CONNECTION = "Connection";

	const char* const CONTENT_BASE = "Content-Base";
	const char* const CONTENT_ENCODING = "Content-Encoding";
	const char* const CONTENT_LANGUAGE = "Content-Language";
	const char* const CONTENT_LENGTH = "Content-Length";
	const char* const CONTENT_LOCATION = "Content-Location";
	const char* const CONTENT_TRANSFER_ENCODING = "Content-Transfer-Encoding";
	const char* const CONTENT_MD5 = "Content-MD5";
	const char* const CONTENT_RANGE = "Content-Range";
	const char* const CONTENT_TYPE = "Content-Type";

	const char* const COOKIE = "Cookie";
	const char* const DATE = "Date";
	const char* const ETAG = "ETag";
	const char* const EXPECT = "Expect";
	const char* const EXPIRES = "Expires";
	const char* const FROM = "From";
	const char* const HOST = "Host";
	const char* const IF_MATCH = "If-Match";
	const char* const IF_MODIFIED_SINCE = "If-Modified-Since";
	const char* const IF_NONE_MATCH = "If-None-Match";
	const char* const IF_RANGE = "If-Range";
	const char* const IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
	const char* const LAST_MODIFIED = "Last-Modified";

	const char* const LOCATION = "Location";
	const char* const MAX_FORWARDS = "Max-Forwards";
	const char* const ORIGIN = "Origin";
	const char* const PRAGMA = "Pragma" ;
	const char* const PROXY_AUTHENTICATE = "Proxy-Authenticate";
	const char* const PROXY_AUTHORIZATION = "Proxy-Authorization";
	const char* const RANGE = "Range";
	const char* const REFERER = "Referer";
	const char* const RETRY_AFTER = "Retry-After";
	const char* const SEC_WEBSOCKET_KEY1 = "Sec-WebSocket-Key1";
	const char* const SEC_WEBSOCKET_KEY2 = "Sec-WebSocket-Key2";
	const char* const SEC_WEBSOCKET_LOCATION = "Sec-WebSocket-Location";
	const char* const SEC_WEBSOCKET_ORIGIN = "Sec-WebSocket-Origin";
	const char* const SEC_WEBSOCKET_PROTOCOL = "Sec-WebSocket-Protocol";
	const char* const SEC_WEBSOCKET_VERSION = "Sec-WebSocket-Version";
	const char* const SEC_WEBSOCKET_KEY = "Sec-WebSocket-Key";
	const char* const SEC_WEBSOCKET_ACCEPT = "Sec-WebSocket-Accept";
	const char* const SEC_WEBSOCKET_EXTENSIONS = "Sec-WebSocket-Extensions";
	const char* const SERVER = "Server";
	const char* const SET_COOKIE = "Set-Cookie";
	const char* const SET_COOKIE2 = "Set-Cookie2";
	const char* const TE = "TE";
	const char* const TRAILER = "Trailer";
	const char* const TRANSFER_ENCODING = "Transfer-Encoding";
	const char* const UPGRADE = "Upgrade";
	const char* const USER_AGENT = "User-Agent";
	const char* const VARY = "Vary";
	const char* const VIA = "Via";
	const char* const WARNING = "Warning";
	const char* const WEBSOCKET_LOCATION = "WebSocket-Location";
	const char* const WEBSOCKET_ORIGIN = "WebSocket-Origin";
	const char* const WEBSOCKET_PROTOCOL = "WebSocket-Protocol";
	const char* const WWW_AUTHENTICATE = "WWW-Authenticate";


	const char* const APPLICATION_X_WWW_FORM_URLENCODED = "application/x-www-form-urlencoded";
	const char* const BASE64 = "base64";
	const char* const BINARY = "binary";
	const char* const BOUNDARY = "boundary";
	const char* const BYTES = "bytes";
	const char* const CHARSET = "charset";
	const char* const CHUNKED = "chunked";
	const char* const CLOSE = "close";
	const char* const COMPRESS = "compress";
	const char* const CONTINUE = "100-continue";
	const char* const DEFLATE = "deflate";
	const char* const GZIP = "gzip";
	const char* const IDENTITY = "identity";
	const char* const KEEP_ALIVE = "keep-alive";
	const char* const MAX_AGE = "max-age";
	const char* const MAX_STALE = "max-stale";
	const char* const MIN_FRESH = "min-fresh";
	const char* const MULTIPART_FORM_DATA = "multipart/form-data";
	const char* const MUST_REVALIDATE = "must-revalidate";
	const char* const NO_CACHE = "no-cache";
	const char* const NO_STORE = "no-store";
	const char* const NO_TRANSFORM = "no-transform";
	const char* const NONE = "none";
	const char* const ONLY_IF_CACHED = "only-if-cached";
	const char* const PRIVATE = "private";
	const char* const PROXY_REVALIDATE = "proxy-revalidate";
	const char* const PUBLIC = "public";
	const char* const QUOTED_PRINTABLE = "quoted-printable";
	const char* const S_MAXAGE = "s-maxage";
	const char* const TRAILERS = "trailers";
	const char* const WEBSOCKET = "WebSocket";
}


}
}

#endif
