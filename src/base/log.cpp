

#include "abb/base/log.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#define SW_LOG_DATE_STRLEN 64
namespace abb{ namespace base{

LogLevel g_min_log_level = LOGLEVEL_INFO;
static FILE * file = NULL;
namespace internal{
	void DefaultLogHandler(LogLevel level,const char* filename, int line, const std::string& message){
		static const char* level_names[] = {"TRACE", "DEBUG", "WARN", "ERROR","FATAL","INFO"};
		if (level >= g_min_log_level) {
			char date_str[SW_LOG_DATE_STRLEN];
			time_t t;
			struct tm *p;
			t = time(NULL);
			p = localtime(&t);
			/*if(!file){
				file = fopen(".\\loger.txt","w");
				if(!file){
					file = stdout;
				}
			}*/
			file = stdout;
			sprintf(date_str, "%d-%02d-%02d %02d:%02d:%02d", p->tm_year + 1900, p->tm_mon+1, p->tm_mday , p->tm_hour, p->tm_min, p->tm_sec);
			fprintf(file, "[%s %s %s:%d] %s\r\n",date_str, level_names[level], filename, line, message.c_str());
			fflush(file);  // Needed on MSVC.
		}
	}
	void NullLogHandler(LogLevel level,const char* filename, int line, const std::string& message){

	}
	static LogHandler * log_handler = &DefaultLogHandler;

	LogRecord::LogRecord(LogLevel level,const char * filename,int line)
				:level_(level),filename_(filename),line_(line),message_(){}

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
		log_handler(this->level_,this->filename_,this->line_,this->message_);
	}
	void LogEnd::operator=(const LogRecord& other){
		const_cast<LogRecord&>(other).End();
	}
}// end namespace internal


LogHandler* SetLogHandler( LogHandler * new_handler){
	LogHandler * old_handler = internal::log_handler;
	internal::log_handler = new_handler;

	if( old_handler == &internal::NullLogHandler){
		old_handler = NULL;
	}

	if(new_handler == NULL){
		internal::log_handler = &internal::NullLogHandler;
	}

	return old_handler;
}

}}
