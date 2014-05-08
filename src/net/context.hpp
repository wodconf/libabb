
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include "abb/net/context_option.hpp"
#include <pthread.h>
#include "loop.hpp"
#include "abb/base/callback.hpp"
#include "abb/net/i_protocol.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context(const ContextOption& option,IProtocolFactory* fac);
	~Context();
	void Run();
	void Stop();
	Loop& GetFreeLoop();
	void Wait();
	IProtocol* GetProtocol(){
		return fac_->CreateProtocol();
	}
	void BackProtocol(IProtocol* val){
		fac_->DestroyProtocol(val);
	}
private:
	pthread_t* threads;
	ContextOption option_;
	Loop* loops_;
	IProtocolFactory* fac_;
	int cur_;
	bool brun;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
