

#ifndef ABB_BASE_BUFFER_HPP_
#define ABB_BASE_BUFFER_HPP_

#include <string>
#include "define.hpp"
#include <stdint.h>
struct iovec;

namespace abb {

class Buffer{
public:
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
	void GaveWr(uint32_t sz){
		wr_+=sz;
		if(wr_ > size_){
			wr_ = size_;
		}
	}
	void GaveRd(uint32_t sz){
		rd_+=sz;
		if(rd_ > wr_){
			rd_ = wr_;
		}
	}
	uint32_t Size(){
		return wr_ - rd_;
	}
	void Back(uint32_t sz){
		if(sz > rd_){
			rd_ = 0;
		}
		rd_-=sz;
	}
	void* Data(){
		return this->buf_+this->rd_;
	}
	void* WrData(){
		return this->buf_+this->wr_;
	}
	uint32_t WrSize(){
		return this->size_ - this->wr_;
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
