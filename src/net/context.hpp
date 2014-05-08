
#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_
#include "abb/net/context_option.hpp"
#include <pthread.h>
#include "loop.hpp"
#include "abb/base/callback.hpp"
namespace abb {
namespace net {

class Context {
public:
	Context(const ContextOption& option);
	~Context();
	void Run();
	void Stop();
	Loop& GetFreeLoop();
	void Wait();
private:
	pthread_t* threads;
	ContextOption option_;
	Loop* loops_;
	int cur_;
	bool brun;
};
extern Context* ctx;
} /* namespace translate */
} /* namespace adcloud */

#endif /* CONTEXT_HPP_ */
