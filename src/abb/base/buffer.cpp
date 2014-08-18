

#include "abb/base/buffer.hpp"
#include <memory.h>
#include <stdlib.h>
#include "abb/base/log.hpp"
#include <sys/uio.h>
#include <string.h>
#include <asm/byteorder.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace abb;

static unsigned long long ntohll(unsigned long long val)
{
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
	{
		return (((unsigned long long )htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
	}
	else if (__BYTE_ORDER == __BIG_ENDIAN)
	{
		return val;
	}else{
		return val;
	}
}

static unsigned long long htonll(unsigned long long val)
{
	if (__BYTE_ORDER == __LITTLE_ENDIAN)
	{
		return (((unsigned long long )htonl((int)((val << 32) >> 32))) << 32) | (unsigned int)htonl((int)(val >> 32));
	}
	else if (__BYTE_ORDER == __BIG_ENDIAN)
	{
		return val;
	}else{
		return val;
	}
}

Buffer::Buffer(){
	this->size_ = 1024;
	this->rd_ = this->wr_ = 0;
	this->buf_ = new char[this->size_];
}
Buffer::~Buffer(){
	delete [] this->buf_;
}

#define DECLARE_OPERATOR_IN_STREAM(TYPE)		\
		Buffer& Buffer::operator<< ( TYPE val){	\
	uint32_t sz = sizeof(TYPE);				\
	if(sz > (size_ - wr_)){			\
		Grow(sz);						\
	}									\
	memcpy(buf_+wr_,&val,sz);\
	wr_+= sz;						\
	return *this;						\
}
#define DECLARE_OPERATOR_OUT_STREAM(TYPE)		\
		Buffer& Buffer::operator>> ( TYPE& val){	\
	unsigned  int sz = sizeof(TYPE);				\
	if(sz >  (wr_ - rd_) ){					\
		throw new Exception("read out of range ");\
		return *this;					\
	}									\
	memcpy(&val,buf_+rd_,sz);\
	rd_+= sz;						\
	return*this;						\
}


DECLARE_OPERATOR_OUT_STREAM(bool)
DECLARE_OPERATOR_OUT_STREAM(int32_t)
DECLARE_OPERATOR_OUT_STREAM(uint32_t)
DECLARE_OPERATOR_OUT_STREAM(int8_t)
DECLARE_OPERATOR_OUT_STREAM(uint8_t)
DECLARE_OPERATOR_OUT_STREAM(int16_t)
DECLARE_OPERATOR_OUT_STREAM(uint16_t)
DECLARE_OPERATOR_OUT_STREAM(int64_t)
DECLARE_OPERATOR_OUT_STREAM(uint64_t)

DECLARE_OPERATOR_IN_STREAM(bool)
DECLARE_OPERATOR_IN_STREAM(int32_t)
DECLARE_OPERATOR_IN_STREAM(uint32_t)
DECLARE_OPERATOR_IN_STREAM(int8_t)
DECLARE_OPERATOR_IN_STREAM(uint8_t)
DECLARE_OPERATOR_IN_STREAM(int16_t)
DECLARE_OPERATOR_IN_STREAM(uint16_t)
DECLARE_OPERATOR_IN_STREAM(int64_t)
DECLARE_OPERATOR_IN_STREAM(uint64_t)

#undef DECLARE_OPERATOR_OUT_STREAM
#undef DECLARE_OPERATOR_IN_STREAM
Buffer& Buffer::operator >> (std::string& str){
	uint32_t cur = rd_;
	char ch = 'a';
	while(true){
		if(cur == wr_){
			break;
		}
		ch = *((char *)buf_+cur++);
		if(ch == '\0'){
			break;
		}
	}
	int sz = cur - rd_;
	if(sz > 0){
		if(ch == '\0'){
			sz -= 1;
		}
		std::string tmp((char *)buf_+rd_,sz);
		str = tmp;
	}
	rd_ = cur;
	return *this;
}
Buffer& Buffer::operator << ( const char* val){
	this->Write((void*)val,strlen(val)+1);
	return *this;
}
Buffer& Buffer::operator <<(const std::string& str){
	this->Write((void*)str.c_str(),str.size());
	char s = '\0';
	this->Write(&s,1);
	return *this;
}
void Buffer::Write(const void*buf,uint32_t sz){
	if(sz ==0){
		return ;
	}
	if(sz > (size_ - wr_)){
		Grow(sz);
	}
	memcpy(buf_+wr_,buf,sz);
	wr_+= sz;
}
uint32_t Buffer::Read(void *buf,uint32_t sz){
	sz = sz > (wr_ - rd_)?(wr_ - rd_):sz;
	memcpy(buf,(char*)buf_+rd_,sz);
	rd_+= sz;
	if(rd_ == wr_){
		rd_ = wr_ = 0;
	}
	return sz ;
}


uint32_t Buffer::ReadToWriter(BufferWriter writer,void*arg){
	int size = wr_ - rd_;
	int ret = 0;
	if(size > 0){
		ret = writer(arg,(void*)(this->buf_+this->rd_),size);
		if(ret > 0){
			this->rd_+=ret;
		}
		return ret > 0?ret:0;
	}
	return 0;
}
uint32_t Buffer::WriteFromeReader(BufferReader reader,void*arg){
	char buf[65535];
	struct iovec io[2];
	int size = size_ - wr_;
	if(size <= 128 ){
		this->Grow(1024);
		size = size_ - wr_;
	}
	io[0].iov_base = (void*)(this->buf_+this->wr_);
	io[0].iov_len = size;
	io[1].iov_base = buf;
	io[1].iov_len = sizeof(buf);
	int ret = reader(arg,io,2);
	if(ret <= size && ret > 0){
		this->wr_+= ret;
	}else if(ret > size){
		this->wr_+= size;
		this->Write(buf,ret - size);
	}
	return ret;
}
void Buffer::Grow(int needsize){
	int fsz = size_ - wr_ + rd_;
	if(fsz < needsize){
		int neddadd = needsize - fsz;
		int add_size = 256;
		while(neddadd > add_size){
			add_size+=256;
		}
		char* nbuf = new char[this->size_+add_size];
		memcpy(nbuf, buf_, size_);
		delete []  buf_;
		buf_ = nbuf;
		this->size_+=add_size;
	}
	memmove(this->buf_,this->buf_+rd_,wr_-rd_);
	this->wr_-= rd_;
	this->rd_ = 0;
}


uint16_t Buffer::HOST_ReadUint16(){
	uint16_t val;
	(*this) >> val;
	return ntohs(val);
}
uint32_t Buffer::HOST_ReadUint32(){
	uint32_t val;
	(*this) >> val;
	return ntohl(val);
}
uint64_t Buffer::HOST_ReadUint64(){
	uint64_t val;
	(*this) >> val;
	return ntohll(val);
}

void Buffer::NET_WriteUint16(uint16_t val){
	val = htons(val);
	(*this) << val;
}
void Buffer::NET_WriteUint32(uint32_t val){
	val = htonl(val);
	(*this) << val;
}
void Buffer::NET_WriteUint64(uint64_t val){
	val = htonll(val);
	(*this) << val;
}
