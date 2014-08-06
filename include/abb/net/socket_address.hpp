#ifndef __ABB_NET_SOCKET_ADDRESS__
#define __ABB_NET_SOCKET_ADDRESS__

#include <string>

struct sockaddr;
namespace abb{
namespace net{

class SocketAddress{
public:
	SocketAddress();
	~SocketAddress();
	SocketAddress(const SocketAddress& other);
	struct sockaddr* GetAddr(){return addr_;}
	unsigned int GetLength();
	bool SetV4(const char *addr, uint16_t port);
	bool SetV6(const char *addr, uint16_t port);
	bool SetUnix(const char *path,unsigned int pathlen);
	bool SetFromAddrInfo(struct addrinfo* a , uint16_t port);
	bool SetFromHostent(struct hostent *ent , uint16_t port);
	bool SetByString(const std::string& str);
	bool SetByStringIgnoreIP(const std::string& str);
private:
	int family_;
	struct sockaddr* addr_;
};

}
}
#endif