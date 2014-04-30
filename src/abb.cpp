

#include "abb/abb.hpp"
#include "net/context.hpp"
namespace abb{

net::Context* NewContext(int num_pool,int num_dis){
	net::Context* ctx = new net::Context();
	ctx->SetNumDispatchThread(num_dis);
	ctx->SetNumPollThread(num_pool);
	ctx->Init();
	return ctx;
}
void RunContext(net::Context* ctx,bool run_cur_thread){
	if(ctx){
		ctx->Run(run_cur_thread);
	}
}
}
