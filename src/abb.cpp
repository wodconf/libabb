

#include "abb/abb.hpp"
#include "net/context.hpp"
namespace abb{

net::Context* NewContext(int num_pool,int num_dis){
	net::Context* ctx = new net::Context();
	ctx->SetNumDispatchThread(4);
	ctx->SetNumPollThread(2);
	ctx->Init();
	return ctx;
}
void RunContext(net::Context* ctx,bool run_cur_thread){
	if(ctx){
		ctx->Run(run_cur_thread);
	}
}
}
