

#ifndef __ABB_BASE_BUFFER_READER_HPP__
#define __ABB_BASE_BUFFER_READER_HPP__

#include <stdint.h>
#include <string>
namespace abb {

class BufferReader {
public:
	BufferReader(void* buf,uint32_t sz);
	~BufferReader();
	BufferReader& operator >> ( bool& val);
	BufferReader& operator >> ( int32_t& val);
	BufferReader& operator >> ( uint32_t& val);
	BufferReader& operator >> ( int8_t& val);
	BufferReader& operator >> ( uint8_t& val);
	BufferReader& operator >> ( int16_t& val);
	BufferReader& operator >> ( uint16_t& val);
	BufferReader& operator >> ( int64_t& val);
	BufferReader& operator >> ( uint64_t& val);
	BufferReader& operator >> (  std::string& val);
	uint32_t Read(void *buf,uint32_t sz);
	uint16_t HOST_ReadUint16();
	uint32_t HOST_ReadUint32();
	uint64_t HOST_ReadUint64();

	uint32_t GetReadSize() const {
		return rd_;
	}
	uint32_t DataSize() const {
		return size_ - rd_;
	}
	uint8_t* DataPtr()const{
		return buf_+rd_;
	}
	void Grow(uint32_t sz){
		rd_ += sz;
		if(rd_ > size_){
			rd_ = size_;
		}
	}
	void Back(uint32_t sz){
		if(sz > rd_){rd_ = 0;}
		else rd_-=sz;
	}
private:
	uint8_t* buf_;
	uint32_t size_;
	uint32_t rd_;
};

}
#endif
