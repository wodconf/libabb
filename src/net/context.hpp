
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include "abb/net/context_option.hpp"
#include <pthread.h>
#include "abb/base/thread_pool.hpp"
#include "loop.hpp"
#include "abb/base/callback.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context(const ContextOption& option,IProtocolFactory* fac);
	~Context();
	void Run(bool run_cur_thread);
	void Stop();
	Loop& GetFreeLoop();
	void Wait();
	IProtocol* GetProtocol(){
		return fac->CreateProtocol();
	}
	void BackProtocol(IProtocol* val){
		fac->DestroyProtocol(val);
	}
private:
	pthread_t* threads;
	ContextOption option_;
	Loop* loops_;
	IProtocolFactory* fac;
	int cur_;
	bool brun;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
