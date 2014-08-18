

#ifndef SOCKET_H_
#define SOCKET_H_

#include "abb/net/ip_addr.hpp"


struct iovec;
namespace abb {
namespace net {

class Socket {
public:
	static bool Listen(int* fd,const IPAddr& addr,int *save_err);
	static bool Connect(int* fd,bool block,const IPAddr& addr,int *save_err,int ms = 5000);
	static bool Accept(int fd,int* outfd,IPAddr*addr,int* save_err);
	static bool Write(int fd,void*buf,int size,int* nwr,int* save_err);
	static bool Read(int fd,void*buf,int size,int* nrd,int*save_err);
	static bool ReadV(int fd,const struct iovec *iov, int iovcnt,int* nrd,int*save_err);
	static void SetNoBlock(int fd,bool b);
	static void SetNoDelay(int fd,bool b);
	static void SetRcvBufSize(int fd,int size);
	static void SetSndBufSize(int fd,int size);
	static void SetRuseAddr(int fd,bool b);
	static void SetKeepAlive(int fd,bool b);
	static bool GetSockError(int fd,int*err);
	static void Close(int fd);
	static bool SetKeepAlive(int fd,bool keppalive,int keep_idle,int keepinterval,int keep_cout);
	static bool ShutDown(int fd,bool read,bool write,int* error);
	static bool SetCloseExec(int fd,bool bset,int*error);
	static void SetRecvTimeout(int fd,int ms);
	static void SetSendTimeout(int fd,int ms);
};

} /* namespace net */
} /* namespace abb */

#endif /* SOCKET_H_ */
