

#include "socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <errno.h>
#include "abb/base/log.hpp"
#include <string.h>
namespace abb {
namespace net {

bool Socket::Write(int fd,void*inbuf,int size,int* nwr,int* save_err){
	int ret;
	*nwr = 0;
	char * buf = (char *)inbuf;
	while(1){
		ret = write(fd,buf,size);
		if(ret < 0){
			int err = errno;
			if( err == EAGAIN){
				break;
			}else if(err == EINTR){
				continue;
			}else{
				if(save_err){
					*save_err = err;
				}
				return false;
			}
		}
		*nwr += ret;
		buf += ret;
		size  = size - ret;
		break;
	}
	return true;
}
bool Socket::ReadV(int fd,const struct iovec *iov, int iovcnt,int* nrd,int*save_err){
	int ret;
	*nrd = 0;
	while(true){
		ret = readv(fd, iov, iovcnt);
		if(ret < 0){
			int err = errno;
			LOG(INFO) << err << strerror(err);
			if(err == EINTR){
				continue;
			}else{
				if(save_err){
					*save_err = err;
				}
				if( err == EAGAIN){
					return true;
				}else{
					return false;
				}
			}
		}else if(ret == 0){
			LOG(INFO) << "0000";
		}else{
			LOG(INFO) << ret;
		}
		*nrd = ret;
		break;
	}
	return true;
}
bool Socket::Read(int fd,void*inbuf,int size,int* nrd,int*save_err){
	int ret;
	*nrd = 0;
	char * buf = (char *)inbuf;
	while(true){
		ret = read(fd,buf,size);
		if(ret < 0){
			int err = errno;
			if(err == EINTR){
				continue;
			}else{
				if(save_err){
					*save_err = err;
				}
				if( err == EAGAIN){
					return true;
				}else{
					return false;
				}
			}
		}
		*nrd+=ret;
		break;
	}
	return true;
}
void Socket::SetRuseAddr(int fd,bool b){

}
void Socket::SetKeepAlive(int fd,bool b){

}

void Socket::SetNoBlock(int fd,bool b){
	int oflag = fcntl(fd, F_GETFL, 0);
	if(b){
		oflag |= O_NONBLOCK;
	}else{
		oflag &= (~O_NONBLOCK);
	}

	if( fcntl(fd, F_SETFL, oflag) < 0){
		//
		LOG(INFO) <<errno << strerror(errno);
	}
}
void Socket::SetNoDelay(int fd,bool b){
	char mode = b?1:0;
	setsockopt( fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&mode, sizeof(char));
}
void Socket::SetRcvBufSize(int fd,int size){
	setsockopt( fd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int));
}
void Socket::SetSndBufSize(int fd,int size){
	setsockopt( fd, SOL_SOCKET, SO_SNDBUF, (const char*)&size, sizeof(int));
}
bool Socket::GetSockError(int fd,int*err){
	unsigned size = sizeof(int);
	return getsockopt(fd,SOL_SOCKET,SO_ERROR,(void*)err,&size) == 0;
}
} /* namespace translate */
} /* namespace adcloud */
