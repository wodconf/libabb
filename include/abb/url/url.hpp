#ifndef __ABB_URL_URL_HPP__
#define __ABB_URL_URL_HPP__

#include <string>

namespace abb{
namespace url{
//scheme:opaque[?query][#fragment]
//scheme://[userinfo@]host/path[?query][#fragment]
//http:://example.com/test
class URL;

extern bool ParseUrl(const std::string& inurl,URL* url);

class URL{
public:
	std::string Scheme;
	std::string Host;// host or host:port
	std::string Path; 
	std::string Opaque;
	std::string RawQuery;//without ?
	std::string Fragment;// without #

	std::string RequestURI();
};
}
}
#endif
