

#ifndef __ABB_BASE_BUFFER_HPP__
#define __ABB_BASE_BUFFER_HPP__

#include <string>
#include "define.hpp"
#include <stdint.h>
#include <exception>
struct iovec;

namespace abb {

class Buffer{
public:
	class Exception:public std::exception{
	public:
		Exception()throw (){}
		Exception(const std::string& str)throw ():str_(str){}
		virtual ~Exception() throw (){}
		virtual const char* what() const throw (){
			return str_.c_str();
		}
		std::string str_;
	};
	typedef int (*BufferWriter)(void*arg,void*buf,int size);
	typedef int (*BufferReader)(void*arg,const struct iovec *iov, int iovcnt);
	Buffer();
	~Buffer();

	Buffer& operator << ( bool val);
	Buffer& operator << ( int32_t val);
	Buffer& operator << ( uint32_t val);
	Buffer& operator << ( int8_t val);
	Buffer& operator << ( uint8_t val);
	Buffer& operator << ( int16_t val);
	Buffer& operator << ( uint16_t val);
	Buffer& operator << ( int64_t val);
	Buffer& operator << ( uint64_t val);
	Buffer& operator << ( const std::string& val);
	Buffer& operator << ( const char* val);
	Buffer& operator >> ( bool& val);
	Buffer& operator >> ( int32_t& val);
	Buffer& operator >> ( uint32_t& val);
	Buffer& operator >> ( int8_t& val);
	Buffer& operator >> ( uint8_t& val);
	Buffer& operator >> ( int16_t& val);
	Buffer& operator >> ( uint16_t& val);
	Buffer& operator >> ( int64_t& val);
	Buffer& operator >> ( uint64_t& val);
	Buffer& operator >> (  std::string& val);
	uint16_t HOST_ReadUint16();
	uint32_t HOST_ReadUint32();
	uint64_t HOST_ReadUint64();
	void NET_WriteUint16(uint16_t);
	void NET_WriteUint32(uint32_t);
	void NET_WriteUint64(uint64_t);

	void Write(const void*buf,uint32_t sz);
	uint32_t Read(void *buf,uint32_t sz);
	void* ReadPtr(){
		return this->buf_+this->rd_;
	}
	uint32_t ReadSize(){
		return wr_ - rd_;
	}
	void GaveRead(uint32_t sz){
		rd_+=sz;
		if(rd_ > wr_){
			rd_ = wr_;
		}
	}
	void BackRead(uint32_t sz){
		if(sz > rd_){
			rd_ = 0;
		}
		rd_-=sz;
	}
	
	void* WritePtr(){
		return this->buf_+this->wr_;
	}
	uint32_t WriteSize(){
		return this->size_ - this->wr_;
	}
	void GaveWrite(uint32_t sz){
		wr_+=sz;
		if(wr_ > size_){
			wr_ = size_;
		}
	}
	void BackWrite(uint32_t sz){
		if( sz > (wr_ -rd_) ){
			wr_ = rd_;
		}
		wr_-=sz;
	}
	void Clear(){
		rd_ = wr_ = 0;
	}
	uint32_t ReadToWriter(BufferWriter writer,void*arg);
	uint32_t WriteFromeReader(BufferReader reader,void*arg);
	void EnoughSize(uint32_t size){
		if((size_ - wr_) >= size){return;}
		else{this->Grow(size);}
	}
	void ResetRd(){
		this->rd_ = 0;
	}
private:
	void Grow(int sz);
	char * buf_;
	uint32_t wr_;
	uint32_t rd_;
	uint32_t size_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Buffer);
};

} /* namespace abb */

#endif /* BUFFER_HPP_ */
