

#include "abb/abb.hpp"
#include "net/context.hpp"
namespace abb{

net::Context* NewContext(const net::ContextOption& option,net::IProtocolFactory* fac){
	net::Context* ctx = new net::Context(option,fac);
	return ctx;
}
void RunContext(net::Context* ctx,bool run_cur_thread){
	if(ctx){
		ctx->Run(run_cur_thread);
	}
}
void DeleteContext(net::Context* ctx){
	if(ctx){
		ctx->WaitAndStop();
		delete ctx;
	}
}
}
