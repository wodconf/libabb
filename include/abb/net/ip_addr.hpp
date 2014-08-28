
#ifndef ABB_NET_ADDR_HPP_
#define ABB_NET_ADDR_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

namespace abb{
namespace net{
class SocketAddress {
public:
	SocketAddress();
	~SocketAddress();
	inline unsigned int Length() const{
		switch (this->family) {
		case AF_UNIX:
			return sizeof(this->sa.nix);
		case AF_INET:
			return sizeof(this->sa.v4);
		case AF_INET6:
			return sizeof(this->sa.v6);
		default:
			return sizeof(this->sa.sa);
		}
	}
	bool SetV4(const char *addr, uint16_t port);
	bool SetV6(const char *addr, uint16_t port);
	bool SetUnix(const char *path,unsigned int pathlen);
	bool SetFromAddrInfo(struct addrinfo* a , uint16_t port);
	bool SetFromHostent(struct hostent *ent , uint16_t port);
	//127.0.0.1:1398
	bool SetByString(const std::string& str);
	bool SetByStringIgnoreIP(const std::string& str);
	std::string ToString() const;
	std::string ToIpString() const;
	int Port() const;
public:
	sa_family_t family;
	union {
		struct sockaddr sa;
		struct sockaddr_un nix;
		struct sockaddr_in v4;
		struct sockaddr_in6 v6;
	} sa;
};

inline bool operator==(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) == 0; }
inline bool operator!=(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) != 0; }
inline bool operator<(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) < 0; }
inline bool operator<=(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) <= 0; }
inline bool operator>(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) > 0; }
inline bool operator>=(const SocketAddress& a, const SocketAddress& b) { return memcmp(&a, &b, sizeof(a)) >= 0; }
}
}



#endif /* ADDR_HPP_ */
