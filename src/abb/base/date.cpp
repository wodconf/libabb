#include "abb/base/date.hpp"
#include <sys/time.h>
namespace abb{
	Date::Date(int64_t u_second):micro_second_(u_second){

	}
	Date::Date():micro_second_(0){
	}
	Date Date::Now(){
		struct timeval time;
		gettimeofday(&time,NULL);
		int64_t tmp =  time.tv_sec;
		tmp*=1000000;
		tmp+=time.tv_usec;
		return Date(tmp);
	}
	std::string Date::FormatString() const{
		char date_str[64];
		time_t t = static_cast<time_t>(micro_second_/1000000);
		struct tm *p = localtime(&t);
		int ms = micro_second_%1000000;
		snprintf(date_str,
				sizeof(date_str), 
				"%d-%02d-%02d %02d:%02d:%02d:%06d", 
				p->tm_year + 1900, 
				p->tm_mon+1, 
				p->tm_mday , 
				p->tm_hour, 
				p->tm_min, 
				p->tm_sec,
				ms);
		return date_str;
	}
}
