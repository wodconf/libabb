#ifndef __ABB_NET_CONTEXT_OPTION__
#define __ABB_NET_CONTEXT_OPTION__ 
#include <stdint.h>
namespace abb{
	namespace net{
	class ContextOption{
	public:
		ContextOption():num_poller_(4),num_dispatch_(1),run_cur_thread_(false){}
		~ContextOption(){};
		void SetNumPoller(uint32_t val){num_poller_ = val;}
		void SetNumDispatch(uint32_t val){num_dispatch_ = val;}
		void SetRunCurrentThread(bool val){run_cur_thread_ = val;}
		uint32_t GetNumPoller(){return num_poller_;}
		uint32_t GetNumDisptch(){return num_dispatch_;}
		bool GetRunCurrentThread(){return run_cur_thread_;}
	private:
		uint32_t num_poller_;
		uint32_t num_dispatch_;
		bool run_cur_thread_;
	};
	}
}
#endif

