
#include "abb/base/buffer_writer.hpp"
#include "abb/base/buffer_exception.hpp"
#include <memory.h>
#include <stdlib.h>
#include <asm/byteorder.h>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace abb {

BufferWriter::BufferWriter(void* buf,uint32_t size)
:buf_((uint8_t*)buf),size_(size),wr_(0)
{
}
BufferWriter::~BufferWriter() {
}

#define SLICE_DECLARE_OPERATOR_IN_STREAM(TYPE)		\
		BufferWriter& BufferWriter::operator<< ( TYPE val){	\
	unsigned int sz = sizeof(TYPE);				\
	if(sz > (size_ - wr_)){			\
		throw BufferException();\
		return *this;					\
	}									\
	TYPE* ptr = (TYPE*)(buf_+wr_);\
	*ptr = val;\
	wr_+= sz;						\
	return *this;						\
}
SLICE_DECLARE_OPERATOR_IN_STREAM(bool)
SLICE_DECLARE_OPERATOR_IN_STREAM(int32_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(uint32_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(int8_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(uint8_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(int16_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(uint16_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(int64_t)
SLICE_DECLARE_OPERATOR_IN_STREAM(uint64_t)


void BufferWriter::Write(void*buf,unsigned int sz){
	if(sz > (size_ - wr_)){
		return;
	}
	memcpy(buf_+wr_,buf,sz);
	wr_+= sz;
}

BufferWriter& BufferWriter::operator <<(const std::string& str){
	this->Write((void*)str.c_str(),(unsigned int)str.size());
	char s = '\0';
	this->Write(&s,1);
	return *this;
}
BufferWriter& BufferWriter::operator << ( const char* val){
	this->Write((void*)val,strlen(val)+1);
	return *this;
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

void BufferWriter::NET_WriteUint16(uint16_t val){
	val = htons(val);
	(*this) << val;
}
void BufferWriter::NET_WriteUint32(uint32_t val){
	val = htonl(val);
	(*this) << val;
}
void BufferWriter::NET_WriteUint64(uint64_t val){
	val = htonll(val);
	(*this) << val;
}
}
