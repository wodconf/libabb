
#ifndef __ABB_BASE_BUFFER_WRITER_HPP__
#define __ABB_BASE_BUFFER_WRITER_HPP__

#include <stdint.h>
#include <string>
namespace abb {

class BufferWriter {
public:
	BufferWriter(void* buf,uint32_t size);
	~BufferWriter();
	BufferWriter& operator << ( bool val);
	BufferWriter& operator << ( int32_t val);
	BufferWriter& operator << ( uint32_t val);
	BufferWriter& operator << ( int8_t val);
	BufferWriter& operator << ( uint8_t val);
	BufferWriter& operator << ( int16_t val);
	BufferWriter& operator << ( uint16_t val);
	BufferWriter& operator << ( int64_t val);
	BufferWriter& operator << ( uint64_t val);
	BufferWriter& operator << ( const std::string& val);
	BufferWriter& operator << ( const char* val);

	void NET_WriteUint16(uint16_t);
	void NET_WriteUint32(uint32_t);
	void NET_WriteUint64(uint64_t);
	void Write(void*buf,uint32_t sz);

	uint32_t WriteSize() const{
		return wr_;
	}
	uint32_t FreeSize() const{
		return size_ -wr_;
	}

	uint8_t* WritePtr() const{
		return buf_+wr_;
	}
	void Grow(uint32_t sz){
		wr_ += sz;
		if(wr_ > size_){
			wr_ = size_;
		}
	}
	void Back(uint32_t sz){
		if(sz > wr_){wr_ = 0;}
		else wr_-=sz;
	}
private:
	uint8_t* buf_;
	uint32_t size_;
	uint32_t wr_;
};

}

#endif
