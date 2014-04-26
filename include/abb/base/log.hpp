
#ifndef _ABB_BASE_LOG_HPP_
#define _ABB_BASE_LOG_HPP_


#include <string>


namespace abb { namespace base{

enum LogLevel {
  LOGLEVEL_INFO,
  LOGLEVEL_WARN,
  LOGLEVEL_ERROR,
  LOGLEVEL_FATAL,
#ifdef NDEBUG
  LOGLEVEL_DFATAL = LOGLEVEL_ERROR
#else
  LOGLEVEL_DFATAL = LOGLEVEL_FATAL
#endif
};
extern LogLevel g_min_log_level;

namespace internal{
	class LogEnd;

	class LogRecord
	{
	public:
		LogRecord(LogLevel level, const char* filename, int line);
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

	};
	class LogEnd{
	public:
		void operator= (const LogRecord&);
	};
}


typedef void LogHandler(LogLevel level,const char* filename, int line, const std::string& message);
LogHandler * SetLogHandler(LogHandler * new_handler);


}}


#define LOG(lvl) ::abb::base::internal::LogEnd() = ::abb::base::internal::LogRecord(::abb::base::LOGLEVEL_##lvl,__FILE__,__LINE__)

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
