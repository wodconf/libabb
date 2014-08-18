

#include "abb/base/log.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#define SW_LOG_DATE_STRLEN 64
namespace abb{

LogLevel g_min_log_level = LOGLEVEL_TRACE;
namespace internal{

	class DefaultLoger:public Loger{
	public:
		DefaultLoger(){}
		~DefaultLoger(){}
		virtual void LogHandler(LogLevel level,const char* filename, int line, const std::string& message){
			static const char* level_names[] = {"TRACE", "DEBUG", "WARN", "ERROR","FATAL","INFO"};
			if (level >= g_min_log_level) {
				char date_str[SW_LOG_DATE_STRLEN];
				time_t t;
				struct tm p;
				t = time(NULL);
				localtime_r(&t, &p);
				snprintf(date_str,SW_LOG_DATE_STRLEN, "%d-%02d-%02d %02d:%02d:%02d", p.tm_year + 1900, p.tm_mon+1, p.tm_mday , p.tm_hour, p.tm_min, p.tm_sec);
				fprintf(stdout, "[%s %s %s:%d] %s\r\n",date_str, level_names[level], filename, line, message.c_str());
				fflush(stdout);  // Needed on MSVC.
			}
		}
	};
	class NullLoger:public Loger{
	public:
		NullLoger(){}
		~NullLoger(){}
		virtual void LogHandler(LogLevel level,const char* filename, int line, const std::string& message){
		}
	};
	static DefaultLoger s_default_loger;
	static NullLoger s_null_loger;
	static Loger * log_handler = &s_default_loger;

	LogRecord::LogRecord(LogLevel level,const char * filename,int line,Loger* loger)
				:level_(level),filename_(filename),line_(line),message_(),loger_(loger){
					if(loger_ == NULL){
						loger_ = log_handler;
					}
				}

	LogRecord::~LogRecord(){}

	LogRecord& LogRecord::operator<<(const std::string& val ){
		this->message_ += val;
		return *this;
	}
	LogRecord& LogRecord::operator<<(const char * val){
		this->message_ += val;
		return *this;
	}
	LogRecord& LogRecord::operator<<( bool b){
		if(b){
			this->message_ += "true";
		}else{
			this->message_ += "false";
		}

		return *this;
	}
	#undef DECLARE_OPERATOR_STREAM
	#define DECLARE_OPERATOR_STREAM(TYPE,FORMAT)		\
		LogRecord& LogRecord::operator<< ( TYPE val){	\
			char buf[128];								\
			sprintf(buf,FORMAT,val);					\
			buf[sizeof(buf)-1] = '\0';					\
			message_ += buf;							\
			return *this;								\
		}
	DECLARE_OPERATOR_STREAM(char,"%c")
	DECLARE_OPERATOR_STREAM(int,"%d")
	DECLARE_OPERATOR_STREAM(unsigned int,"%u")
	DECLARE_OPERATOR_STREAM(long,"%ld")
	DECLARE_OPERATOR_STREAM(unsigned long,"%lu")
	DECLARE_OPERATOR_STREAM(long long,"%lld")
	DECLARE_OPERATOR_STREAM(unsigned long long,"%llu")
	DECLARE_OPERATOR_STREAM(double,"%g")
	#undef DECLARE_OPERATOR_STREAM


	void LogRecord::End(){
		if(loger_)
			loger_->LogHandler(this->level_,this->filename_,this->line_,this->message_);
	}
	void LogEnd::operator=(const LogRecord& other){
		const_cast<LogRecord&>(other).End();
	}
}// end namespace internal

Loger* SetLoger(Loger* new_loger){
	Loger * old_loger = internal::log_handler;
	internal::log_handler = new_loger;
	if( old_loger == &internal::s_default_loger){
		old_loger = NULL;
	}
	if(new_loger == NULL){
		internal::log_handler = &internal::s_null_loger;
	}
	return old_loger;
}

}
