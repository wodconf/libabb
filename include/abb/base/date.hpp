#ifndef __ABB_BASE_DATE_HPP__
#define __ABB_BASE_DATE_HPP__

#include <stdint.h>
#include <string>

namespace abb{
class Date{
public:
	static Date Now();
public:
	Date(uint64_t micro_second);
	Date();
	std::string FormatString() const;
	uint64_t MicroSecond() const { return micro_second_;};
	uint64_t MilliSecond() const { return micro_second_/1000;};
private:
	uint64_t micro_second_;	
};
}

#endif
