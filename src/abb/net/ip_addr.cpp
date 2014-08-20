

#include "abb/net/ip_addr.hpp"
#include <stdlib.h>
#include <sstream>

static bool CheckIp(const std::string& host){
	unsigned int i=0,count = 0;
	size_t len = host.size();
	for ( ; i < len ; i++ ) {
		if ( !(host[i] >= '0' && host[i] <= '9' )) {
			if ( host[i] == '.' ) {
				count++;
				continue;
			}
			return false;
		}
	}
	return  (count == 3 );
}

using namespace abb::net;
IPAddr::IPAddr():family(0){
	memset(this,0,sizeof(IPAddr));
}
IPAddr::~IPAddr(){

}
bool IPAddr::SetV4(const char *addr, uint16_t port){
	int res = 1;

	if (addr) {
		if( !CheckIp(addr) ){
			struct hostent* htt = gethostbyname(addr);
			if(!htt) return false;
			return this->SetFromHostent(htt,port);
		}
		res = inet_pton(AF_INET, addr, &this->sa.v4.sin_addr);
	}
	else {
		sa.v4.sin_addr.s_addr = INADDR_ANY;
	}
	if (res > 0) {
		this->family = AF_INET;
		this->sa.v4.sin_family = AF_INET;
		this->sa.v4.sin_port = htons(port);
		return true;
	}
	return false;
}
bool IPAddr::SetV6(const char *addr, uint16_t port){
	int res = 1;
	struct addrinfo *ai, hints;
	if (!addr) {
		/* the memset above zeroes the struct, which is the same
		 * bit pattern as the any address
		 */
		family = AF_INET6;
		sa.v6.sin6_family = AF_INET6;
		sa.v6.sin6_port = htons(port);
		return false;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_NUMERICHOST;
	hints.ai_family = AF_INET6;

	res = getaddrinfo(addr, NULL, &hints, &ai);
	if (res) {
		return false;
	}

	family = ai->ai_family;

	switch (family) {
	case AF_INET6:
		memcpy(&sa.sa, ai->ai_addr, sizeof(sa.v6));
		sa.v6.sin6_port = htons(port);
		freeaddrinfo(ai);
		return false;

	case AF_INET:
		memcpy(&sa.sa, ai->ai_addr, sizeof(sa.v4));
		sa.v4.sin_port = htons(port);
		freeaddrinfo(ai);
		return true;

	default:
		freeaddrinfo(ai);
		return false;
	}
}
bool IPAddr::SetUnix(const char *path,unsigned int pathlen){
	if (!path) {
		return false;
	}
	if (*path == 0) {
		/* explicitly do not support the Linux unix abstract namespace */
		return false;
	}
	if (pathlen == 0) {
		pathlen = strlen(path);
	}
	/* Linux defines UNIX_PATH_MAX, but there is no POSIX requirement.
	 * Let's just ask the compiler for the size */
	if (pathlen + 1 /* \0 */ >= sizeof(this->sa.nix.sun_path)) {
		return false;
	}

	memset(&sa, 0, sizeof(sa));
	this->family = AF_UNIX;
	this->sa.nix.sun_family = AF_UNIX;

	memcpy(&this->sa.nix.sun_path, path, pathlen);
	return true;
}
bool IPAddr::SetFromAddrInfo(struct addrinfo* ai, uint16_t port){
	this->family = ai->ai_family;
	switch (this->family) {
	case AF_INET6:
		memcpy(&this->sa.sa, ai->ai_addr, sizeof(this->sa.v6));
		sa.v6.sin6_port = htons(port);
		return true;

	case AF_INET:
		memcpy(&this->sa.sa, ai->ai_addr, sizeof(this->sa.v4));
		sa.v4.sin_port = htons(port);
		return true;

	default:
		return false;
	}
}
static bool ParseAddr(const std::string& addr,std::string *ip,int *port){
	std::string::size_type index = addr.find(':');
	if(index != std::string::npos){
		*ip = addr.substr(0,index);
		std::string str_port = addr.substr(index+1);
		*port = atoi(str_port.c_str());
		return true;
	}
	return false;
}
bool IPAddr::SetByString(const std::string& str){
	std::string ip;
	int port;
	if(ParseAddr(str,&ip,&port)){
		return this->SetV4(ip.c_str(),port);
	}
	return false;
}
bool IPAddr::SetByStringIgnoreIP(const std::string& str){
	std::string ip;
	int port;
	if(ParseAddr(str,&ip,&port)){
		return this->SetV4(NULL,port);
	}
	return false;
}
/*
 * const char * inet_ntop(
int fm,//AF_INET AF_INET6
const void *addrptr,//sockaddr_in.sin_addr sockaddr_in6.sin6_addr
char *strptr,
size_t len//INET_ADDRSTRLEN(16) INET6_ADDRSTRLEN(46)
);
 */
int IPAddr::Port()const{
	if(this->family == AF_INET6){
		return ntohs( sa.v6.sin6_port);
	}else if(this->family == AF_INET){
		return ntohs( sa.v4.sin_port);
	}
	return -1;
}
std::string IPAddr::ToString() const{
	if(this->family == AF_INET6){
		char ip[46];
		inet_ntop(this->family, (void *)&sa.v6.sin6_addr, ip, 46);
		std::ostringstream s;
		s << ip << ":" << ntohs( sa.v6.sin6_port);
		return s.str();
	}else if(this->family == AF_INET){
		char ip[16];
		inet_ntop(this->family, (void *)&sa.v4.sin_addr, ip, 16);
		std::ostringstream s;
		s << ip << ":" << ntohs( sa.v4.sin_port);
		return s.str();
	}
	return "";
}
std::string IPAddr::ToIpString() const{
	if(this->family == AF_INET6){
		char ip[46];
		inet_ntop(this->family, (void *)&sa.v6.sin6_addr, ip, 46);
		return ip;
	}else if(this->family == AF_INET){
		char ip[16];
		inet_ntop(this->family, (void *)&sa.v4.sin_addr, ip, 16);
		return ip;
	}
	return "";
}
bool IPAddr::SetFromHostent(struct hostent *ent, uint16_t port)
{
	this->family = ent->h_addrtype;

	switch (this->family) {
	case AF_INET6:
		this->sa.v6.sin6_family = this->family;
		memcpy(&this->sa.v6.sin6_addr, ent->h_addr_list[0],
				sizeof(this->sa.v6.sin6_addr));
		sa.v6.sin6_port = htons(port);
		return true;

	case AF_INET:
		this->sa.v4.sin_family = this->family;
		memcpy(&this->sa.v4.sin_addr, ent->h_addr_list[0],
				sizeof(this->sa.v4.sin_addr));
		sa.v4.sin_port = htons(port);
		return true;

	default:
		return false;
	}
}
