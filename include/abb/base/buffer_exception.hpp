
#ifndef __ABB_BASE_BUFFER_EXCEPTION_HPP__
#define __ABB_BASE_BUFFER_EXCEPTION_HPP__

#include <exception>

namespace abb{
class BufferException:public std::exception{
public:
	BufferException()throw (){}
	virtual ~BufferException() throw (){}
	virtual const char* what() const throw (){
		return "OUT OF RANGE";
	}
};
}



#endif
