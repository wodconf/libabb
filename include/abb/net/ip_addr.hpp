/*
 * addr.hpp
 *
 *  Created on: 2014��4��25��
 *      Author: goalworld
 */

#ifndef ADDR_HPP_
#define ADDR_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
namespace abb{
namespace net{

}
}
class IPAddr {
public:
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
	bool SetFromAddrInfo(struct addrinfo* a);
	bool SetFromHostent(struct hostent *ent);
public:
  sa_family_t family;
  union {
    struct sockaddr sa;
    struct sockaddr_un nix;
    struct sockaddr_in v4;
    struct sockaddr_in6 v6;
  } sa;
};


#endif /* ADDR_HPP_ */
