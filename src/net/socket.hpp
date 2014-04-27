

#ifndef SOCKET_H_
#define SOCKET_H_
#include <unistd.h>
namespace abb {
namespace net {

class Socket {
public:
	static bool Write(int fd,void*buf,int size,int* nwr,int* save_err);
	static bool Read(int fd,void*buf,int size,int* nrd,int*save_err);
	static void SetNoBlock(int fd,bool b);
	static void SetNoDelay(int fd,bool b);
	static void SetRcvBufSize(int fd,int size);
	static void SetSndBufSize(int fd,int size);
	static void SetRuseAddr(int fd,bool b);
	static void SetKeepAlive(int fd,bool b);
	static bool GetSockError(int fd,int*err);
};

} /* namespace net */
} /* namespace abb */

#endif /* SOCKET_H_ */
