

#ifndef ABB_BASE_BUFFER_HPP_
#define ABB_BASE_BUFFER_HPP_

#include <string>
#include "define.hpp"

struct iovec;

namespace abb {
namespace base {

class Buffer{
public:
	typedef int (*BufferWriter)(void*arg,void*buf,int size);
	typedef int (*BufferReader)(void*arg,const struct iovec *iov, int iovcnt);
	Buffer();
	~Buffer();
	Buffer& operator << ( bool val);
	Buffer& operator << ( int val);
	Buffer& operator << ( unsigned int val);
	Buffer& operator << ( char val);
	Buffer& operator << ( unsigned char val);
	Buffer& operator << ( short val);
	Buffer& operator << ( unsigned short val);
	Buffer& operator << ( long val);
	Buffer& operator << ( unsigned long val);
	Buffer& operator << ( long long val);
	Buffer& operator << ( unsigned long long val);
	Buffer& operator << ( const std::string& val);
	Buffer& operator << ( const char* val);
	Buffer& operator >> ( bool& val);
	Buffer& operator >> ( int& val);
	Buffer& operator >> ( unsigned int& val);
	Buffer& operator >> ( char& val);
	Buffer& operator >> ( unsigned char& val);
	Buffer& operator >> ( short& val);
	Buffer& operator >> ( unsigned short& val);
	Buffer& operator >> ( long& val);
	Buffer& operator >> ( unsigned long& val);
	Buffer& operator >> ( long long& val);
	Buffer& operator >> ( unsigned long long& val);
	Buffer& operator >> (  std::string& val);

	void Write(void*buf,unsigned int sz);
	unsigned int Read(void *buf,unsigned int sz);
	void GaveWr(int sz){
		wr_+=sz;
		if(wr_ > size_){
			wr_ = size_;
		}
	}
	void GaveRd(int sz){
		rd_+=sz;
		if(rd_ > wr_){
			rd_ = wr_;
		}
	}
	unsigned int Size(){
		return wr_ - rd_;
	}
	void Back(unsigned int sz){
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
	void Clear(){
		rd_ = wr_ = 0;
	}
	unsigned int ReadToWriter(BufferWriter writer,void*arg);
	unsigned int WriteFromeReader(BufferReader reader,void*arg);
	void EnoughSize(unsigned int size){
		if((size_ - wr_) >= size){return;}
		else{this->Grow(size);}
	}
	void ResetRd(){
		this->rd_ = 0;
	}
private:
	void Grow(int sz);
	char * buf_;
	unsigned int wr_;
	unsigned int rd_;
	unsigned int size_;
	ABB_BASE_DISALLOW_COPY_AND_ASSIGN(Buffer);
};

} /* namespace base */
} /* namespace abb */

#endif /* BUFFER_HPP_ */
