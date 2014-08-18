
#ifndef _ABB_BASE_LOG_HPP_
#define _ABB_BASE_LOG_HPP_


#include <string>


namespace abb {

enum LogLevel {
	LOGLEVEL_TRACE,
	LOGLEVEL_DEBUG,
  	LOGLEVEL_WARN,
  	LOGLEVEL_ERROR,
  	LOGLEVEL_FATAL,
	LOGLEVEL_INFO
};
extern LogLevel g_min_log_level;

class Loger{
public:
	Loger(){}
	virtual ~Loger(){}
	virtual void LogHandler(LogLevel level,const char* filename, int line, const std::string& message) = 0;
};
namespace internal{
	class LogEnd;
	
	class LogRecord
	{
	public:
		LogRecord(LogLevel level, const char* filename, int line,Loger* loger);
		~LogRecord();
		LogRecord& operator<< (const std::string& val);
		LogRecord& operator<< (const char* val);
		LogRecord& operator<< (int val);
		LogRecord& operator<< (char val);
		LogRecord& operator<< (unsigned int val);
		LogRecord& operator<< (long val);
		LogRecord& operator<< (unsigned long val);
		LogRecord& operator<< (long long val);
		LogRecord& operator<< (unsigned long long val);
		LogRecord& operator<< (double val);
		LogRecord& operator<<( bool b);
	private:
		friend class LogEnd;
		void End();
		LogLevel level_;
		const char * filename_;
		int line_;
		std::string message_;
		Loger* loger_;

	};
	class LogEnd{
	public:
		void operator= (const LogRecord&);
	};
}


Loger* SetLoger(Loger* loger);

}

#define LOG_OF(lvl,loger) ::abb::internal::LogEnd() = ::abb::internal::LogRecord(::abb::LOGLEVEL_##lvl,__FILE__,__LINE__,loger)

#define LOG(lvl) ::abb::internal::LogEnd() = ::abb::internal::LogRecord(::abb::LOGLEVEL_##lvl,__FILE__,__LINE__,NULL)

#define LOG_IF(lvl,condition) !(condition)?void(0) : LOG(lvl)

#define CHECK(EXPRESSION)\
	LOG_IF(FATAL, !(EXPRESSION)) << "CHECK failed:" #EXPRESSION":"

#define CHECK_EQ(A,B) CHECK((A) == (B))
#define CHECK_NE(A,B) CHECK((A) != (B))
#define CHECK_LT(A,B) CHECK((A) < (B))
#define CHECK_LE(A,B) CHECK((A) <= (B))
#define CHECK_GT(A,B) CHECK((A) > (B))
#define CHECK_GE(A,B) CHECK((A) >= (B))


#endif
