

#include "abb/abb.hpp"
#include "net/context.hpp"
namespace abb{
void Init(){
	if(!net::ctx){
		net::ctx = new net::Context();
		net::ctx->SetNumDispatchThread(4);
		net::ctx->SetNumPollThread(2);
		net::ctx->Init();
	}
}
void Run(){
	if(net::ctx){
		net::ctx->Run();
	}
}
}
