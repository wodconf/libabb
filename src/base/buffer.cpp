

#include "abb/base/buffer.hpp"
#include <memory.h>
#include <stdlib.h>
#include "abb/base/log.hpp"
#include <sys/uio.h>
using namespace abb::base;
Buffer::Buffer(){
	this->size_ = 1024;
	this->rd_ = this->wr_ = 0;
	this->buf_ = new char[this->size_];
}
Buffer::~Buffer(){
	delete this->buf_;
}

#define DECLARE_OPERATOR_IN_STREAM(TYPE)		\
		Buffer& Buffer::operator<< ( TYPE val){	\
	unsigned int sz = sizeof(TYPE);				\
	if(sz > (size_ - wr_)){			\
		Grow(sz);						\
	}									\
	memcpy(buf_+wr_,&val,sz);\
	wr_+= sz;						\
	return *this;						\
}
#define DECLARE_OPERATOR_OUT_STREAM(TYPE)		\
		Buffer& Buffer::operator>> ( TYPE val){	\
	unsigned  int sz = sizeof(TYPE);				\
	if(sz >  (wr_ - rd_) ){					\
		return *this;					\
	}									\
	memcpy(&val,buf_+rd_,sz);\
	rd_+= sz;						\
	return*this;						\
}
DECLARE_OPERATOR_IN_STREAM(bool)
DECLARE_OPERATOR_IN_STREAM(int)
DECLARE_OPERATOR_IN_STREAM(unsigned int)
DECLARE_OPERATOR_IN_STREAM(char)
DECLARE_OPERATOR_IN_STREAM(unsigned char)
DECLARE_OPERATOR_IN_STREAM(short)
DECLARE_OPERATOR_IN_STREAM(unsigned short)
DECLARE_OPERATOR_IN_STREAM(long)
DECLARE_OPERATOR_IN_STREAM(unsigned long)
DECLARE_OPERATOR_IN_STREAM(long long)
DECLARE_OPERATOR_IN_STREAM(unsigned long long)

DECLARE_OPERATOR_OUT_STREAM(bool&)
DECLARE_OPERATOR_OUT_STREAM(int&)
DECLARE_OPERATOR_OUT_STREAM(unsigned int&)
DECLARE_OPERATOR_OUT_STREAM(char&)
DECLARE_OPERATOR_OUT_STREAM(unsigned char&)
DECLARE_OPERATOR_OUT_STREAM(short&)
DECLARE_OPERATOR_OUT_STREAM(unsigned short&)
DECLARE_OPERATOR_OUT_STREAM(long&)
DECLARE_OPERATOR_OUT_STREAM(unsigned long&)
DECLARE_OPERATOR_OUT_STREAM(long long&)
DECLARE_OPERATOR_OUT_STREAM(unsigned long long&)

Buffer& Buffer::operator >> (std::string& str){
	unsigned int cur = rd_;
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
Buffer& Buffer::operator <<(const std::string& str){
	this->Write((void*)str.c_str(),str.size());
	char s = '\0';
	this->Write(&s,1);
	return *this;
}
void Buffer::Write(void*buf,unsigned int sz){
	if(sz > (size_ - wr_)){
		Grow(sz);
	}
	memcpy(buf_+wr_,buf,sz);
	wr_+= sz;
}
unsigned int Buffer::Read(void *buf,unsigned int sz){
	sz = sz > (wr_ - rd_)?(wr_ - rd_):sz;
	rd_+= sz;
	if(rd_ == wr_){
		rd_ = wr_ = 0;
	}
	memcpy(buf,(char*)buf_+rd_,sz);
	return sz ;
}


void Buffer::ReadToWriter(BufferWriter writer,void*arg){
	int size = wr_ - rd_;
	int ret = 0;
	if(size > 0){
		ret = writer(arg,(void*)(this->buf_+this->rd_),size);
		if(ret > 0){
			this->rd_+=ret;
		}
	}
}
void Buffer::WriteFromeReader(BufferReader reader,void*arg){
	char buf[65535];
	struct iovec io[2];
	int size = size_ - wr_;
	if(size == 128 ){
		this->Grow(1024);
		size = size_ - wr_;
	}
	io[0].iov_base = (void*)(this->buf_+this->wr_);
	io[0].iov_len = size;
	io[1].iov_base = buf;
	io[1].iov_len = sizeof(buf);
	int ret = reader(arg,io,2);
	if(ret <= size){
		this->wr_+= ret;
	}else if(ret > size){
		this->Write(buf,ret - size);
	}
}
void Buffer::Grow(int needsize){
	int fsz = size_ - wr_ + rd_;
	if(fsz < needsize){
		int neddadd = needsize - fsz;
		int add_size = 256;
		while(neddadd > add_size){
			add_size+=256;
		}
		char* new_buf = new char[this->size_+add_size];
		memcpy(new_buf, this->buf_+rd_,wr_-rd_);
		delete []buf_;
		this->buf_ = (char*)new_buf;
		this->size_+=add_size;
		this->wr_-= rd_;
		this->rd_ = 0;
		return;
	}
	memmove(this->buf_,this->buf_+rd_,wr_-rd_);
	this->wr_-= rd_;
	this->rd_ = 0;
}

