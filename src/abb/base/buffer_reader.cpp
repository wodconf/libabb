

#include "abb/base/buffer_reader.hpp"
#include "abb/base/buffer_exception.hpp"
#include <memory.h>
#include <stdlib.h>
#include <asm/byteorder.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <abb/base/log.hpp>
namespace abb {


BufferReader::BufferReader(void* buf,uint32_t size)
:buf_((uint8_t*)buf),size_(size),rd_(0){
}

BufferReader::~BufferReader() {
}

#define SLICE_DECLARE_OPERATOR_OUT_STREAM(TYPE)		\
BufferReader& BufferReader::operator>> ( TYPE& val){	\
	unsigned  int sz = sizeof(TYPE);				\
	if(sz >  (size_ - rd_) ){					\
		throw BufferException();\
		return *this;					\
	}									\
	TYPE* ptr =(TYPE*) (buf_+rd_);\
	val = *ptr;\
	rd_+= sz;						\
	return*this;						\
}
SLICE_DECLARE_OPERATOR_OUT_STREAM(bool)
SLICE_DECLARE_OPERATOR_OUT_STREAM(int32_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(uint32_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(int8_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(uint8_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(int16_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(uint16_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(int64_t)
SLICE_DECLARE_OPERATOR_OUT_STREAM(uint64_t)

BufferReader& BufferReader::operator >> (std::string& str){
	unsigned int cur = rd_;
	char ch = '\0';
	while(true){
		if(cur == size_){
			break;
		}
		ch = *(buf_+cur++);
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
	//LOG(DEBUG) << "BufferReader" << str << "size:" << sz << "size_:" << size_ <<  "rd:"<< rd_ << "ch:" << ch;
	return *this;
}
uint32_t BufferReader::Read(void *buf,uint32_t sz){
	if(!buf)return 0;
	sz = sz > (size_ - rd_)?(size_ - rd_):sz;
	memcpy(buf,(char*)buf_+rd_,sz);
	rd_+= sz;
	return sz ;
}

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

uint16_t BufferReader::HOST_ReadUint16(){
	uint16_t val;
	(*this) >> val;
	return ntohs(val);
}
uint32_t BufferReader::HOST_ReadUint32(){
	uint32_t val;
	(*this) >> val;
	return ntohl(val);
}
uint64_t BufferReader::HOST_ReadUint64(){
	uint64_t val;
	(*this) >> val;
	return ntohll(val);
}
}
