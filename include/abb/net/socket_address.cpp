

#include "abb/net/socket_address.hpp"
#include <stdlib.h>
#include <sstream>
//
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>


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
using namespace abb::net;
SocketAddress::SocketAddress():family(0){
	addr_ = NULL;
}
SocketAddress::~SocketAddress(){
	if(addr_){
		delete addr_;
	}
}
unsigned int SocketAddress::GetLength(){
	switch (this->family) {
	case AF_UNIX:
		return sizeof(struct sockaddr_un);
	case AF_INET:
		return sizeof(struct sockaddr_in);
	case AF_INET6:
		return sizeof(struct sockaddr_in6);
	default:
		return 0;
	}
}
bool SocketAddress::SetV4(const char *addr, uint16_t port){
	int res = 1;
	struct sockaddr_in v4;
	if (addr) {
		if( !CheckIp(addr) ){
			struct hostent* htt = gethostbyname(addr);
			if(!htt) return false;
			return this->SetFromHostent(htt,port);
		}
		res = inet_pton(AF_INET, addr, &v4.sin_addr);
	}
	else {
		v4.sin_addr.s_addr = INADDR_ANY;
	}
	if (res > 0) {
		this->family = AF_INET;
		v4.sin_family = AF_INET;
		v4.sin_port = htons(port);
		addr_ = (sockaddr*)( new struct sockaddr_in());
		memcpy(addr_,&v4,sizeof(v4));
		return true;
	}
	return false;
}
bool SocketAddress::SetV6(const char *addr, uint16_t port){
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
		return true;

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
bool SocketAddress::SetUnix(const char *path,unsigned int pathlen){
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
	struct sockaddr_un nix;
	/* Linux defines UNIX_PATH_MAX, but there is no POSIX requirement.
	 * Let's just ask the compiler for the size */
	if (pathlen + 1 /* \0 */ >= sizeof(nix.sun_path)) {
		return false;
	}
	this->family = AF_UNIX;
	nix.sun_family = AF_UNIX;

	memcpy(&nix.sun_path, path, pathlen);
	addr_ = (sockaddr*)( new struct sockaddr_un());
	memcpy(addr_,&nix,sizeof(nix));
	return true;
}
bool SocketAddress::SetFromAddrInfo(struct addrinfo* ai, uint16_t port){
	this->family = ai->ai_family;
	switch (this->family) {
	case AF_INET6:
		struct sockaddr_in6* v6 = new struct sockaddr_in6();
		memcpy(v6, ai->ai_addr, sizeof(*v6));
		v6->sin6_port = htons(port);
		addr_ = v6;
		return true;

	case AF_INET:
		struct sockaddr_in* v4 = new struct sockaddr_in();
		memcpy(v4, ai->ai_addr, sizeof(*v4));
		v4->sin_port = htons(port);
		addr_ = v4;
		return true;
	default:
		return false;
	}
}

bool SocketAddress::SetByString(const std::string& str){
	std::string ip;
	int port;
	if(ParseAddr(str,&ip,&port)){
		return this->SetV4(ip.c_str(),port);
	}
	return false;
}
bool SocketAddress::SetByStringIgnoreIP(const std::string& str){
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
std::string SocketAddress::ToString() const{
	if(this->family == AF_INET6){
		struct sockaddr_in6* v6 = static_cast<struct sockaddr_in6*>(addr_);
		char ip[46];
		inet_ntop(this->family, (void *)&v6->sin6_addr, ip, 46);
		std::ostringstream s;
		s << ip << ":" << ntohs( v6->sin6_port);
		return s.str();
	}else if(this->family == AF_INET){
		struct sockaddr_in* v4 = static_cast<struct sockaddr_in*>(addr_);
		char ip[16];
		inet_ntop(this->family, (void *)&v4->sin_addr, ip, 16);
		std::ostringstream s;
		s << ip << ":" << ntohs( v4->sin_port);
		return s.str();
	}
	return "";
}
bool SocketAddress::SetFromHostent(struct hostent *ent, uint16_t port)
{
	this->family = ent->h_addrtype;

	switch (this->family) {
	case AF_INET6:
		struct sockaddr_in6* v6 = new struct sockaddr_in6();
		v6->sin6_family = this->family;
		memcpy(&v6->sin6_addr, ent->h_addr_list[0],
				sizeof(v6->sin6_addr));
		v6->sin6_port = htons(port);
		addr_ = v6;
		return true;

	case AF_INET:
		struct sockaddr_in* v4 = new struct sockaddr_in();
		v4->sin_family = this->family;
		memcpy(&v4->sin_addr, ent->h_addr_list[0],
				sizeof(v4->sin_addr));
		v4->sin_port = htons(port);
		addr_ = v4;
		return true;

	default:
		return false;
	}
}
