

#include "abb/abb.hpp"
#include "net/context.hpp"
namespace abb{

net::Context* NewContext(const net::ContextOption& option){
	net::Context* ctx = new net::Context(option);
	return ctx;
}
void RunContext(net::Context* ctx){
	if(ctx){
		ctx->Run();
	}
}
void DeleteContext(net::Context* ctx){
	if(ctx){
		ctx->Stop();
		ctx->Wait();
		delete ctx;
	}
}
}
