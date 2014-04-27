


#include "abb/net/ip_addr.hpp"
#include <memory>
using namespace abb::net;
bool IPAddr::SetV4(const char *addr, uint16_t port){
	int res = 1;

	if (addr) {
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
bool IPAddr::SetFromAddrInfo(struct addrinfo* ai){
	this->family = ai->ai_family;
	switch (this->family) {
	case AF_INET6:
		memcpy(&this->sa.sa, ai->ai_addr, sizeof(this->sa.v6));
		return true;

	case AF_INET:
		memcpy(&this->sa.sa, ai->ai_addr, sizeof(this->sa.v4));
		return true;

	default:
		return false;
	}
}

bool IPAddr::SetFromHostent(struct hostent *ent)
{
	this->family = ent->h_addrtype;

	switch (this->family) {
	case AF_INET6:
		this->sa.v6.sin6_family = this->family;
		memcpy(&this->sa.v6.sin6_addr, ent->h_addr_list[0],
				sizeof(this->sa.v6.sin6_addr));
		return true;

	case AF_INET:
		this->sa.v4.sin_family = this->family;
		memcpy(&this->sa.v4.sin_addr, ent->h_addr_list[0],
				sizeof(this->sa.v4.sin_addr));
		return true;

	default:
		return false;
	}
}
